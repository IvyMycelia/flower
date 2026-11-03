#!/bin/sh
set -eu

mkdir -p output
integer_index_dir=$(mktemp -d ./output/integer-indices.XXXXXX)

for integer_type in i8 u8 i16 u16 i32 u32 i64 u64; do
    result_base="$integer_index_dir/$integer_type"

    cat >"$result_base.flo" <<FLO
type Index = $integer_type
type Values = u64[2]

func main(): int
    index: Index = 1
    zero: Index = 0
    values: Values = [0, 18446744073709551615]

    if values[index] != 18446744073709551615:
        return 1
    end

    values[index] = 42
    if values[index] != 42:
        return 1
    end

    ptr: @u64 = new u64
    ptr[zero] = 18446744073709551615
    if ptr[zero] != 18446744073709551615:
        prune ptr
        return 1
    end
    prune ptr

    text: string = "AB"
    if text[index] != 'B':
        return 1
    end

    length: i32 = 2
    dynamic: u8[length]
    dynamic[index] = 255
    if dynamic[index] != 255:
        return 1
    end

    enabled: bool = false
    if values[enabled as u8] != 0:
        return 1
    end

    letter: char = 'A'
    if values[(letter as i32) - 65] != 0:
        return 1
    end

    print("Integer indexing passed.\n")
    return 0
end
FLO

    if ! ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
        cat "$result_base.log"
        printf 'FAIL: index fixture did not compile: %s\n' "$integer_type"
        exit 1
    fi

    if ! "$result_base" >"$result_base.actual" 2>"$result_base.runtime.log"; then
        cat "$result_base.runtime.log"
        printf 'FAIL: index fixture failed at runtime: %s\n' "$integer_type"
        exit 1
    fi

    printf 'Integer indexing passed.\n' >"$result_base.expected"

    if ! cmp -s "$result_base.expected" "$result_base.actual"; then
        diff -u "$result_base.expected" "$result_base.actual" || :
        exit 1
    fi

    printf 'PASS: %s indices\n' "$integer_type"
done

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$integer_index_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: invalid index fixture accepted: %s\n' "$rejection_name"
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
        printf 'FAIL: missing index diagnostic: %s\n' "$rejection_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: rejection occurred outside typechecking: %s\n' "$rejection_name"
        exit 1
    fi

    if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: invalid index reached code generation: %s\n' "$rejection_name"
        exit 1
    fi

    printf 'PASS: %s\n' "$rejection_name"
}

while IFS='|' read -r case_name declaration; do
    for operation in array_read array_write pointer_read pointer_write string_read; do
        case "$operation" in
            array_read) statement='print(values[index])' ;;
            array_write) statement='values[index] = 1' ;;
            pointer_read) statement='print(ptr[index])' ;;
            pointer_write) statement='ptr[index] = 1' ;;
            string_read) statement='print(text[index])' ;;
        esac

        expect_rejection "${case_name}_${operation}" 'subscript requires an integer type' <<FLO
func main(): int
    values: u8[2] = [0, 1]
    ptr: @u8 = new u8
    text: string = "AB"
    $declaration
    $statement
    prune ptr
    return 0
end
FLO
    done
done <<'CASES'
bool|index: bool = false
char|index: char = 'A'
float|index: float = 0.0
string|index: string = "0"
pointer|index: @u8 = ptr
array|index: i32[1] = [0]
nullable|index: ?i32 = 0
CASES

expect_rejection union_index 'subscript requires an integer type' <<'FLO'
func main(): int
    values: u8[2] = [0, 1]
    index: i8 | u8 = 0 as u8
    print(values[index])
    return 0
end
FLO

expect_rejection unknown_index 'could not resolve subscript index' <<'FLO'
func main(): int
    values: u8[2] = [0, 1]
    print(values[missing])
    return 0
end
FLO

expect_rejection unknown_object 'could not resolve indexed object' <<'FLO'
func main(): int
    print(missing[0])
    return 0
end
FLO

expect_rejection string_write 'string indexing is readonly' <<'FLO'
func main(): int
    text: string = "AB"
    index: u64 = 0
    text[index] = 'C'
    return 0
end
FLO

printf 'Integer subscript type checks passed.\n'