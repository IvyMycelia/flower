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

    if ! grep -F "$expected_message" "$result_base.log" >/dev/null; then
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

printf 'Integer type diagnostics passed.\n'