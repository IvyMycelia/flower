#!/bin/sh
set -eu

mkdir -p output
array_length_dir=$(mktemp -d ./output/array-lengths.XXXXXX)
positive_base="$array_length_dir/positive"

cat >"$positive_base.flo" <<'FLO'
type MaximumLength = u8[2147483647]
type PaddedMaximumLength = u8[0002147483647]
type Triple = u8[0003]

func main(): int
    values: Triple = [1, 2, 3]
    inferred: u8[] = [4, 5]

    if values[0] != 1 or values[2] != 3:
        return 1
    end

    if inferred[0] != 4 or inferred[1] != 5:
        return 1
    end

    print("Array length parsing passed.\n")
    return 0
end
FLO

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: valid array lengths did not compile\n'
    exit 1
fi

if ! "$positive_base" >"$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: array-length fixture failed at runtime\n'
    exit 1
fi

printf 'Array length parsing passed.\n' >"$positive_base.expected"

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    exit 1
fi

while IFS='|' read -r case_name length diagnostic; do
    result_base="$array_length_dir/$case_name"

    cat >"$result_base.flo" <<FLO
type InvalidLength = u8[$length]

func main(): int
    return 0
end
FLO

    if ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
        printf 'FAIL: invalid array length accepted: %s\n' "$case_name"
        exit 1
    else
        compiler_status=$?
    fi

    if [ "$compiler_status" -ne 1 ]; then
        cat "$result_base.log"
        printf 'FAIL: expected parser rejection, got status %s\n' "$compiler_status"
        exit 1
    fi

    if ! grep -F "$diagnostic" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing array-length diagnostic: %s\n' "$case_name"
        exit 1
    fi

    if ! grep -F 'Parse failed with' "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing parser-failure summary\n'
        exit 1
    fi

    if grep -F 'Checking types..' "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: invalid array length reached typechecking\n'
        exit 1
    fi

    if [ -e "$result_base.c" ]; then
        printf 'FAIL: invalid array length produced C output\n'
        exit 1
    fi

    printf 'PASS: %s\n' "$case_name"
done <<'CASES'
zero|0|array length must be greater than zero
padded_zero|000000|array length must be greater than zero
above_limit|2147483648|array length exceeds compiler limit of 2147483647 elements
padded_above_limit|0002147483648|array length exceeds compiler limit of 2147483647 elements
u64_max|18446744073709551615|array length exceeds compiler limit of 2147483647 elements
beyond_u64|18446744073709551616|array length exceeds compiler limit of 2147483647 elements
huge|99999999999999999999999999999999999999999999999999|array length exceeds compiler limit of 2147483647 elements
CASES

printf 'Checked array-length parsing passed.\n'