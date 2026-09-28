#!/bin/sh
set -eu

mkdir -p output
cast_test_dir=$(mktemp -d ./output/integer-casts.XXXXXX)

compile_and_run() {
    source_file=$1
    result_base=$2

    if ! ./bin/Flower "$source_file" "$result_base" >"$result_base.compile.log" 2>&1; then
        cat "$result_base.compile.log"
        printf 'FAIL: cast fixture did not compile: %s\n' "$source_file"
        exit 1
    fi

    if ! "$result_base" >"$result_base.actual" 2>"$result_base.runtime.log"; then
        cat "$result_base.runtime.log"
        printf 'FAIL: cast fixture failed at runtime: %s\n' "$source_file"
        exit 1
    fi
}

check_output() {
    result_base=$1

    if ! cmp -s "$result_base.expected" "$result_base.actual"; then
        printf 'FAIL: integer cast output differs\n'
        diff -u "$result_base.expected" "$result_base.actual" || :
        exit 1
    fi
}

unsigned_maximum() {
    case "$1" in
        8) printf '%s' 255;;
        16) printf '%s' 65535 ;;
        32) printf '%s' 4294967295 ;;
        64) printf '%s' 18446744073709551615 ;;
        *) exit 1 ;;
    esac
}

result_base="$cast_test_dir/boundaries"
cat >"$result_base.expected" <<'EXPECTED'
44
-1
1
44
44
-1
255
-1
18446744073709551615
9223372036854775808
-1
127
44
1
false
true
EXPECTED

compile_and_run ./examples/types/integer_casts.flo "$result_base"
check_output "$result_base"

matrix_source="$cast_test_dir/matrix.flo"
matrix_base="$cast_test_dir/matrix"

printf 'func main(): int\n' >"$matrix_source"
: >"$matrix_base.expected"

for source_type in i8 u8 i16 u16 i32 u32 i64 u64; do
    source_width=${source_type#?}

    case "$source_type" in
        i*) source_value=-1 ;;
        u*) source_value=$(unsigned_maximum "$source_width")
    esac

    printf ' source_%s: %s = %s\n' \
        "$source_type" "$source_type" "$source_value" >>"$matrix_source"

    for target_type in i8 u8 i16 u16 i32 u32 i64 u64; do
        target_width=${target_type#?}
            
        case "$source_type:$target_type" in
            i*:i*)
                expected=-1
                ;;
            i*:u*)
                expected=$(unsigned_maximum "$target_width")
                ;;
            u*:i*)
                if [ "$target_width" -le "$source_width" ]; then
                    expected=-1
                else
                    expected=$(unsigned_maximum "$source_width")
                fi
                ;;
            u*:u*)
                if [ "$target_width" -le "$source_width" ]; then
                    expected=$(unsigned_maximum "$target_width")
                else
                    expected=$(unsigned_maximum "$source_width")
                fi
                ;;
        esac
    
        printf '    print(source_%s as %s)\n    print("\\n")\n' \
            "$source_type" "$target_type" >>"$matrix_source"
        printf '%s\n' "$expected" >>"$matrix_base.expected"
    done

    printf '    print((source_%s as bool) as %s)\n  print("\\n")\n' \
        "$source_type" "$source_type" >>"$matrix_source"
    printf '1\n' >>"$matrix_base.expected"

    printf '    source_%s = 0\n' "$source_type" >>"$matrix_source"
    printf '    print((source_%s as bool) as %s)\n  print("\\n")\n' \
        "$source_type" "$source_type" >>"$matrix_source"
    printf '0\n' >>"$matrix_base.expected"
done

printf '    return 0\nend\n' >>"$matrix_source"

compile_and_run "$matrix_source" "$matrix_base"
check_output "$matrix_base"

while IFS='|' read -r case_name source_type target_type; do
    source_file="$cast_test_dir/$case_name.flo"
    result_base="$cast_test_dir/$case_name"

    printf 'func main(): int\n  source: %s = 1\n  target: %s = source\n   return 0\nend\n' \
        "$source_type" "$target_type" >"$source_file"

    if ./bin/Flower "$source_file" "$result_base" >"$result_base.log" 2>&1; then
        printf 'FAIL: implicit integer conversion was accepted: %s\n' "$case_name"
        exit 1
    fi

    if ! grep -F "variable initializer does not match declared type" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing implicit-conversion diagnostics: %s\n' "$case_name"
        exit 1
    fi

    if ! grep -F "Typecheck failed" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        exit 1
    fi

    if grep -F "Codegen..." "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: invalid conversion reached code generation\n'
        exit 1
    fi
done <<'CASES'
implicit_widening|u8|u16
implicit_narrowing|u16|u8
implicit_signedness|u8|i8
CASES

printf 'Integer casts passed: boundaries, 64 type pairs, bool conversions, and implicit rejection.\n'
