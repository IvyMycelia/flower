#!/bin/sh
set -eu

mkdir -p output
conversion_test_dir=$(mktemp -d ./output/integer-conversions.XXXXXX)

while IFS='|' read -r case_name source_type target_type reason; do
    for context in local global assignment argument return array_write field_write array_literal struct_literal pointer_write; do
        result_base="$conversion_test_dir/${case_name}_${context}"
        global_declaration=''
        extra_function=''
        statement=''

        case "$context" in
            local)
                statement='initialized: Target = source'
                ;;
            global)
                global_declaration='global_target: Target = global_source'
                ;;
            assignment)
                statement='target = source'
                ;;
            argument)
                statement='accept(source)'
                ;;
            return)
                extra_function='func bad_return(value: Source): Target
    return value
end'
                ;;
            array_write)
                statement='values[0] = source'
                ;;
            field_write)
                statement='box.value = source'
                ;;
            array_literal)
                statement='initialized: Target[1] = [source]'
                ;;
            struct_literal)
                statement='initialized_box: Box = {source}'
                ;;
            pointer_write)
                statement='@pointer = source'
                ;;
        esac

        cat >"$result_base.flo" <<FLO
type Source = $source_type
type Target = $target_type

struct Box {
    value: Target
}

global_source: Source = 1
$global_declaration

func accept(value: Target): Target
    return value
end

$extra_function

func main(): int
    source: Source = 1
    target: Target
    values: Target[1]
    box: Box
    pointer: @Target = new Target

    $statement

    prune pointer
    return 0
end
FLO

        if ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
            printf 'FAIL: implicit conversion accepted: %s/%s\n' "$case_name" "$context"
            exit 1
        else
            compiler_status=$?
        fi

        case "$compiler_status" in
            1|255) ;;
            *)
                cat "$result_base.log"
                printf 'FAIL: abnormal compiler exit %s\n' "$compiler_status"
                exit 1
                ;;
        esac

        expected_message="$reason from $source_type to $target_type; use an explicit cast"

        if ! grep -F "$expected_message" "$result_base.log" >/dev/null; then
            cat "$result_base.log"
            printf 'FAIL: missing conversion detail: %s/%s\n' "$case_name" "$context"
            exit 1
        fi

        if ! grep -F 'Typecheck failed' "$result_base.log" >/dev/null; then
            cat "$result_base.log"
            printf 'FAIL: conversion failed outside typechecking\n'
            exit 1
        fi

        if grep -F 'Codegen...' "$result_base.log" >/dev/null; then
            cat "$result_base.log"
            printf 'FAIL: invalid conversion reached code generation\n'
            exit 1
        fi

        printf 'PASS: %s/%s\n' "$case_name" "$context"
    done
done <<'CASES'
widening|u8|u16|implicit integer widening
narrowing|u16|u8|implicit integer narrowing
signedness|u8|i8|implicit integer signedness change
widening_signedness|i8|u16|implicit integer widening with signedness change
narrowing_signedness|u16|i8|implicit integer narrowing with signedness change
wide_narrowing_signedness|u64|i32|implicit integer narrowing with signedness change
CASES

printf 'Integer conversion diagnostics passed.\n'