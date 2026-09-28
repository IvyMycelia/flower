#!/bin/sh
set -eu

mkdir -p output
parser_test_dir=$(mktemp -d ./output/parser-errors.XXXXXX)

expect_parse_error() {
    source_file=$1
    result_base=$2

    if ./bin/Flower "$source_file" "$result_base" >"$result_base.log" 2>&1; then
        cat "$result_base.log"
        printf 'FAIL: malformed source was accepted: %s\n' "$source_file"
        exit 1
    else
        compiler_status=$?
    fi

    if [ "$compiler_status" -ne 1 ]; then
        cat "$result_base.log"
        printf 'FAIL: expected normal parser rejection, got status %s: %s\n' \
            "$compiler_status" "$source_file"
        exit 1
    fi

    if ! grep -F "Parse failed with" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing parser-failure summary\n'
        exit 1
    fi

    if grep -F "Checking types.." "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: malformed source reached typechecking\n'
        exit 1
    fi

    if grep -F "Codegen..." "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: malformed source reached code generation\n'
        exit 1
    fi

    if [ -e "$result_base.c" ]; then
        printf 'FAIL: malformed source produced a C file\n'
        exit 1
    fi

    printf 'PASS: %s\n' "$source_file"
}

for body_kind in function while if else for; do
    source_file="$parser_test_dir/$body_kind.flo"

    printf 'func main(): int\n' >"$source_file"

    case "$body_kind" in
        function) ;;
        while) printf ' while true:\n' >>"$source_file" ;;
        if) printf '    if true:\n' >>"$source_file" ;;
        else) printf '  if false:\n return 0\n  else:\n' >>"$source_file" ;;
        for) printf '   for i in 0..1:\n' >>"$source_file" ;;
    esac

    printf '    source: u8 1\n  target: u16 = source\n' >>"$source_file"

    if [ "$body_kind" != function ]; then
        printf '    end\n' >>"$source_file"
    fi

    printf '    return 0\nend\n' >>"$source_file"

    expect_parse_error "$source_file" "$parser_test_dir/$body_kind"
done

cat >"$parser_test_dir/top_level.flo" <<'FLO'
source: u8 1

func main(): int
    return 0
end
FLO

expect_parse_error \
    "$parser_test_dir/top_level.flo" \
    "$parser_test_dir/top_level"

cat >"$parser_test_dir/expression.flo" <<'FLO'
func main(): int
    value: int = ()
    return 0
end
FLO

expect_parse_error \
    "$parser_test_dir/expression.flo" \
    "$parser_test_dir/expression"

cat >"$parser_test_dir/unterminated_for.flo" <<'FLO'
func main(): int
    for i in 0..1:
        value: int = 0
FLO

expect_parse_error \
    "$parser_test_dir/unterminated_for.flo" \
    "$parser_test_dir/unterminated_for"

cat >"$parser_test_dir/import_error.flo" <<'FLO'
import "./function.flo"

func main(): int
    return 0
end
FLO

expect_parse_error \
    "$parser_test_dir/import_error.flo" \
    "$parser_test_dir/import_error"

printf 'Parser error handling passed.\n'
