#!/bin/sh
set -eu

mkdir -p output
sizeof_result_dir=$(mktemp -d ./output/sizeof-result.XXXXXX)
positive_base="$sizeof_result_dir/positive"

cat >"$positive_base.flo" <<'FLO'
global_size: u64 = sizeof(u64)

func accept_size(value: u64): u64
    return value
end

func make_size(): u64
    return sizeof(i64)
end

func main(): int
    size: u64 = sizeof(u64)
    expected: u64 = 8

    if size != expected or global_size != expected:
        return 1
    end

    if accept_size(sizeof(u64)) != expected:
        return 1
    end

    if make_size() != expected:
        return 1
    end

    next_size: u64 = sizeof(u64) + 1
    if next_size != 9:
        return 1
    end

    explicit_small: i32 = sizeof(i32) as i32
    if explicit_small != 4:
        return 1
    end

    values: u8[5] = [0, 0, 0, 0, 42]
    if values[sizeof(i32)] != 42:
        return 1
    end

    print(sizeof(i8))
    print("\n")
    print(sizeof(u8))
    print("\n")
    print(sizeof(i16))
    print("\n")
    print(sizeof(u16))
    print("\n")
    print(sizeof(i32))
    print("\n")
    print(sizeof(u32))
    print("\n")
    print(sizeof(i64))
    print("\n")
    print(sizeof(u64))
    print("\n")
    print(sizeof(int))
    print("\n")

    return 0
end
FLO

cat >"$positive_base.expected" <<'EXPECTED'
1
1
2
2
4
4
8
8
4
EXPECTED

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: sizeof result fixture did not compile\n'
    exit 1
fi

if ! "$positive_base" >"$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: sizeof result fixture failed at runtime\n'
    exit 1
fi

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    exit 1
fi

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$sizeof_result_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: implicit sizeof conversion accepted: %s\n' "$rejection_name"
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
        printf 'FAIL: missing sizeof diagnostic: %s\n' "$rejection_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: rejection occurred outside typechecking\n'
        exit 1
    fi

    if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: invalid sizeof conversion reached code generation\n'
        exit 1
    fi

    printf 'PASS: %s\n' "$rejection_name"
}

expect_rejection assignment 'variable initializer does not match declared type' <<'FLO'
func main(): int
    size: i32 = sizeof(i32)
    return 0
end
FLO

expect_rejection argument 'function argument does not match parameter type' <<'FLO'
func accept(value: i32): int
    return value
end

func main(): int
    return accept(sizeof(i32))
end
FLO

expect_rejection return_value 'return value does not match function return type' <<'FLO'
func make_size(): i32
    return sizeof(i32)
end

func main(): int
    return 0
end
FLO

printf 'sizeof u64 result checks passed.\n'