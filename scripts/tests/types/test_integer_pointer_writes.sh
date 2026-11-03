#!/bin/sh
set -eu

mkdir -p output
pointer_write_dir=$(mktemp -d ./output/integer-pointer-writes.XXXXXX)

while IFS='|' read -r integer_type minimum maximum; do
    result_base="$pointer_write_dir/$integer_type"

    cat >"$result_base.flo" <<FLO
type Value = $integer_type
type Pointer = @Value

func main(): int
    ptr: Pointer = new Value

    @ptr = $minimum
    print(@ptr)
    print("\n")

    @ptr = $maximum
    print(@ptr)
    print("\n")

    @ptr = $maximum + 0
    print(@ptr)
    print("\n")

    typed: Value = 20
    @ptr = typed
    print(@ptr)
    print("\n")

    prune ptr
    return 0
end
FLO

    printf '%s\n' "$minimum" "$maximum" "$maximum" 20 >"$result_base.expected"

    if ! ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
        cat "$result_base.log"
        printf 'FAIL: pointer-write fixture did not compile: %s\n' "$integer_type"
        exit 1
    fi

    if ! "$result_base" >"$result_base.actual" 2>"$result_base.runtime.log"; then
        cat "$result_base.runtime.log"
        printf 'FAIL: pointer-write fixture failed at runtime: %s\n' "$integer_type"
        exit 1
    fi

    if ! cmp -s "$result_base.expected" "$result_base.actual"; then
        diff -u "$result_base.expected" "$result_base.actual" || :
        printf 'FAIL: pointer-write output differs: %s\n' "$integer_type"
        exit 1
    fi

    printf 'PASS: pointer writes for %s\n' "$integer_type"
done <<'TYPES'
i8|-128|127
u8|0|255
i16|-32768|32767
u16|0|65535
i32|-2147483648|2147483647
u32|0|4294967295
i64|-9223372036854775808|9223372036854775807
u64|0|18446744073709551615
TYPES

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$pointer_write_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: invalid pointer write accepted: %s\n' "$rejection_name"
        exit 1
    else
        rejection_status=$?
    fi

    case "$rejection_status" in
        1|255) ;;
        *)
            cat "$rejection_base.log"
            printf 'FAIL: abnormal compiler exit %s: %s\n' "$rejection_status" "$rejection_name"
            exit 1
            ;;
        esac

        if ! grep -F "$rejection_message" "$rejection_base.log" >/dev/null; then
            cat "$rejection_base.log"
            printf 'FAIL: missing pointer-write diagnostic: %s\n' "$rejection_name"
            exit 1
        fi

        if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
            cat "$rejection_base.log"
            printf 'FAIL: rejection occurred outside typechecking: %s\n' "$rejection_name"
            exit 1
        fi

        if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
            cat "$rejection_base.log"
            printf 'FAIL: invalid pointer write reached code generation: %s\n' "$rejection_name"
            exit 1
        fi

        printf 'PASS: %s\n' "$rejection_name"
}

while IFS='|' read -r case_name integer_type expression diagnostic; do
    expect_rejection "$case_name" "$diagnostic" <<FLO
func main(): int
    ptr: @$integer_type = new $integer_type
    @ptr = $expression
    prune ptr
    return 0
end
FLO
done <<'CASES'
u8_above|u8|256|integer literal outside range of u8
u8_negative|u8|-1|negative integer literal cannot initialize u8
i8_below|i8|-129|integer literal outside range of i8
u16_above|u16|65536|integer literal outside range of u16
u32_above|u32|4294967296|integer literal outside range of u32
i64_below|i64|-9223372036854775809|integer literal outside range of i64
u64_above|u64|18446744073709551616|integer literal outside range of u64
CASES

while IFS='|' read -r case_name destination source_type; do
    expect_rejection "$case_name" 'dereference assignment value does not match pointee type' <<FLO
func main(): int
    ptr: @$destination = new $destination
    value: $source_type = 20
    @ptr = value
    prune ptr
    return 0
end
FLO
done <<'CASES'
implicit_narrowing|u8|u16
implicit_widening|u16|u8
implicit_signedness|u8|i8
CASES

expect_rejection non_pointer 'dereference assignment requires a concrete pointer type' <<'FLO'
func main(): int
    value: u8 = 0
    @value = 20
    return 0
end
FLO

expect_rejection unknown_pointer 'unknown variable in dereference assignment' <<'FLO'
func main(): int
    @missing = 20
    return 0
end
FLO

expect_rejection unknown_value 'could not resolve value in dereference assignment' <<'FLO'
func main(): int
    ptr: @u8 = new u8
    @ptr = missing
    prune ptr
    return 0
end
FLO

printf 'Integer pointer-write checks passed.\n'