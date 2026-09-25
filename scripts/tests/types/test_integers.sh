#!/bin/sh
set -eu

mkdir -p output

expect_type_error() {
    source_file=$1
    result_base=$2
    expected_message=$3

    if ./bin/Flower "$source_file" "$result_base" >"$result_base.log" 2>&1; then
        cat "$result_base.log"
        printf 'FAIL: expected Flower typechecking to reject %s\n' "$source_file"
        return 1
    fi

    if ! grep -F "Typecheck failed" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: %s failed outside the expected typecheck path\n' "$source_file"
        return 1
    fi

    if ! grep -F "$expected_message" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing expected error message for %s\n' "$source_file"
        return 1
    fi

    if grep -F "Codegen..." "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: code generation ran for invalid input %s\n' "$source_file"
        return 1
    fi

    printf 'PASS: %s\n' "$source_file"
}

expect_type_error \
    ./examples/types/integer_duplicate.flo \
    ./output/integer_duplicate \
    "duplicate canonical integer member: i32"

expect_type_error \
    ./examples/types/integer_duplicate_alias.flo \
    ./output/integer_duplicate_alias \
    "duplicate canonical integer member: i32"

literal_test_dir=$(mktemp -d ./output/integer-ranges.XXXXXX)

while IFS='|' read -r case_name target_type literal expected_message; do
    if [ -z "$case_name" ]; then
        continue
    fi

    source_file="$literal_test_dir/$case_name.flo"
    result_base="$literal_test_dir/$case_name"

    printf 'func main(): int\n  value: %s = %s\n    return 0\nend\n' \
        "$target_type" "$literal" >"$source_file"

    expect_type_error "$source_file" "$result_base" "$expected_message"
done <<'CASES'
i8_below|i8|-129|integer literal outside range of i8
i8_above|i8|128|integer literal outside range of i8
u8_above|u8|256|integer literal outside range of u8
u8_negative|u8|-1|negative integer literal cannot initialize u8

i16_below|i16|-32769|integer literal outside range of i16
i16_above|i16|32768|integer literal outside range of i16
u16_above|u16|65536|integer literal outside range of u16
u16_negative|u16|-1|negative integer literal cannot initialize u16

i32_below|i32|-2147483649|integer literal outside range of i32
i32_above|i32|2147483648|integer literal outside range of i32
int_above|int|2147483648|integer literal outside range of i32
u32_above|u32|4294967296|integer literal outside range of u32
u32_negative|u32|-1|negative integer literal cannot initialize u32

i64_below|i64|-9223372036854775809|integer literal outside range of i64
i64_above|i64|9223372036854775808|integer literal outside range of i64
u64_above|u64|18446744073709551616|integer literal outside range of u64
u64_negative|u64|-1|negative integer literal cannot initialize u64

beyond_u64|u64|999999999999999999999999999999|integer literal outside range of u64
CASES

printf 'func main(): int\n  print(2147483648)\n return 0\nend\n' \
    >"$literal_test_dir/default_positive.flo"

expect_type_error \
    "$literal_test_dir/default_positive.flo" \
    "$literal_test_dir/default_positive" \
    "integer literal outside default i32 range"

printf 'func main(): int\n  print(-2147483649)\n    return 0\nend\n' \
    >"$literal_test_dir/default_negative.flo"

expect_type_error \
    "$literal_test_dir/default_negative.flo" \
    "$literal_test_dir/default_negative" \
    "integer literal outside default i32 range"

printf 'Integer type diagnostics passed.\n'