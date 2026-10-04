#!/bin/sh
set -eu

mkdir -p output
arithmetic_test_dir=$(mktemp -d ./output/integer_arithmetic.XXXXXX)

compile_case() {
    source_file=$1
    result_base=$2

    if ! ./bin/Flower "$source_file" "$result_base" >"$result_base.compile.log" 2>&1; then
        cat "$result_base.compile.log"
        printf 'FAIL: arithmetic fixture did not compile: %s\n' "$source_file"
        exit 1
    fi
}

check_program() {
    result_base=$1

    if ! "$result_base" >"$result_base.actual" 2>"$result_base.runtime.log"; then
        cat "$result_base.runtime.log"
        printf 'FAIL: arithmetic fixture failed at runtime\n'
        exit 1
    fi

    if ! cmp -s "$result_base.expected" "$result_base.actual"; then
        diff -u "$result_base.expected" "$result_base.actual" || :
        printf 'FAIL: arithmetic output differs\n'
        exit 1
    fi
}

result_base="$arithmetic_test_dir/integration"

cat >"$result_base.expected" <<'EXPECTED'
0
-128
6148914691236517205
0
-2
2
254
2
1
1
1
2
0
EXPECTED

compile_case ./examples/types/integer_arithmetic.flo "$result_base"
check_program "$result_base"

cat >"$arithmetic_test_dir/types.txt" <<'TYPES'
i8|-128|127|-2|-1|-128
u8|0|255|254|255|0
i16|-32768|32767|-2|-1|-32768
u16|0|65535|65534|65535|0
i32|-2147483648|2147483647|-2|-1|-2147483648
u32|0|4294967295|4294967294|4294967295|0
i64|-9223372036854775808|9223372036854775807|-2|-1|-9223372036854775808
u64|0|18446744073709551615|18446744073709551614|18446744073709551615|0
TYPES

while IFS='|' read -r integer_type minimum maximum twice_maximum negative_one wrapped; do
    source_file="$arithmetic_test_dir/$integer_type.flo"
    result_base="$arithmetic_test_dir/$integer_type"

    cat >"$source_file" <<FLO
func main(): int
    low: $integer_type = $minimum
    high: $integer_type = $maximum
    one: $integer_type = 1
    two: $integer_type = 2
    three: $integer_type = 3

    print(high + one)
    print("\n")
    print(low - one)
    print("\n")
    print(high * two)
    print("\n")
    print(-one)
    print("\n")
    print(-low)
    print("\n")
    print(three / two)
    print("\n")
    print(high + (3 - 2))
    print("\n")
    print(high > low)
    print("\n")

    contextual: $integer_type = $maximum + 1
    print(contextual)
    print("\n")

    return 0
end
FLO

    printf '%s\n' \
        "$wrapped" "$maximum" "$twice_maximum" "$negative_one" \
        "$minimum" 1 "$wrapped" true "$wrapped" >"$result_base.expected"

    compile_case "$source_file" "$result_base"
    check_program "$result_base"
done <"$arithmetic_test_dir/types.txt"

while IFS='|' read -r integer_type minimum remainder; do
    for trap_kind in zero overflow; do
        case "$integer_type:$trap_kind" in
            u*:overflow) continue ;;
        esac

        if [ "$trap_kind" = zero ]; then
            left_value=1
            right_value=0
            expected_message='integer division by zero'
        else
            left_value=$minimum
            right_value=-1
            expected_message='signed integer division overflow'
        fi

        source_file="$arithmetic_test_dir/trap_${integer_type}_${trap_kind}.flo"
        result_base="$arithmetic_test_dir/trap_${integer_type}_${trap_kind}"

        cat >"$source_file" <<FLO
func main(): int
    left: $integer_type = $left_value
    right: $integer_type = $right_value
    print(left / right)
    return 0
end
FLO

        compile_case "$source_file" "$result_base"

        if "$result_base" >"$result_base.stdout" 2>"$result_base.stderr"; then
            printf 'FAIL: division did not trap: %s\n' "$source_file"
            exit 1
        else
            runtime_status=$?
        fi

        if [ "$runtime_status" -ne 1 ]; then
            cat "$result_base.stderr"
            printf 'FAIL: unexpected trap status: %s\n' "$runtime_status"
            exit 1
        fi

        if ! grep -F "$expected_message" "$result_base.stderr" >/dev/null; then
            cat "$result_base.stderr"
            printf 'FAIL: missing division-trap diagnostic\n'
            exit 1
        fi
    done
done <"$arithmetic_test_dir/types.txt"

while IFS='|' read -r case_name left_type right_type operator expected_message; do
    source_file="$arithmetic_test_dir/$case_name.flo"
    result_base="$arithmetic_test_dir/$case_name"

    cat >"$source_file" <<FLO
func main(): int
    left: $left_type = 1
    right: $right_type = 2
    print(left $operator right)
    return 0
end
FLO

    if ./bin/Flower "$source_file" "$result_base" >"$result_base.log" 2>&1; then
        printf 'FAIL: mixed integer operands were accepted: %s\n' "$case_name"
        exit 1
    fi

    if ! grep -F "$expected_message" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing mixed-integer diagnostics\n'
        exit 1
    fi

    if ! grep -F "Typecheck failed" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        exit 1
    fi

    if grep -F "Codegen..." "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: mixed integer operands reached code generation\n'
        exit 1
    fi
done <<'CASES'
mixed_width_add|u8|u16|+|mixed-width arithmetic: u8 and u16
mixed_sign_add|i8|u8|+|mixed-signedness arithmetic: i8 and u8
mixed_width_compare|i8|i16|<|mixed-width comparison: i8 and i16
mixed_sign_compare|i8|u8|<|mixed-signedness comparison: i8 and u8
CASES

printf 'Integer arithmetic, comparisons, and division traps passed.\n'