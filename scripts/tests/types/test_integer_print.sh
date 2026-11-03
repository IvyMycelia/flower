#!/bin/sh
set -eu

mkdir -p output
print_test_dir=$(mktemp -d ./output/integer-print.XXXXXX)
result_base="$print_test_dir/program"

if ! ./bin/Flower ./examples/io/integer_print.flo "$result_base" \
    >"$print_test_dir/compile.log" 2>&1; then
    cat "$print_test_dir/compile.log"
    printf 'FAIL: integer printing fixture did not compile\n'
    exit 1
fi

if "$result_base" >"$print_test_dir/actual.txt" 2>"$print_test_dir/runtime.log"; then
    :
else
    runtime_status=$?
    cat "$print_test_dir/runtime.log"
    printf 'FAIL: integer condition test exited with status %s\n' "$runtime_status"
    exit 1
fi

cat >"$print_test_dir/expected.txt" <<'EXPECTED'
-128 127 0 255
-32768 32767 0 65535
-2147483648 2147483647 0 4294967295
-9223372036854775808 9223372036854775807 0 18446744073709551615
-2147483648 2147483647
A true
EXPECTED

if ! cmp -s "$print_test_dir/expected.txt" "$print_test_dir/actual.txt"; then
    printf 'FAIL: integer printing output differs\n'
    diff -u "$print_test_dir/expected.txt" "$print_test_dir/actual.txt" || :
    exit 1
fi

printf 'Integer printing and conditions passed.\n'