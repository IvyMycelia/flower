#!/bin/sh
set -eu

mkdir -p output
sizeof_operand_dir=$(mktemp -d ./output/sizeof-operands.XXXXXX)
positive_base="$sizeof_operand_dir/positive"

cat >"$positive_base.flo" <<'FLO'
type Triple = u64[3]
type TripleAlias = Triple
type Huge = u64[536870912]

global_size: u64 = sizeof(TripleAlias)
global_arithmetic: u64 = sizeof(TripleAlias) + 1
global_cast: i32 = sizeof(u64) as i32
global_large: u64 = sizeof(Huge)
global_large_arithmetic: u64 = sizeof(Huge) + 1

func parameter_size(values: u16[3]): u64
    return sizeof(values)
end

func main(): int
    fixed: TripleAlias = [1, 2, 3]
    inferred: u16[] = [4, 5, 6]
    pointer: @u16 = &inferred[0]

    if sizeof(i8[3]) != 3 or sizeof(u8[3]) != 3:
        return 1
    end
    if sizeof(i16[3]) != 6 or sizeof(u16[3]) != 6:
        return 1
    end
    if sizeof(i32[3]) != 12 or sizeof(u32[3]) != 12:
        return 1
    end
    if sizeof(i64[3]) != 24 or sizeof(u64[3]) != 24:
        return 1
    end

    if sizeof(TripleAlias) != 24 or sizeof(fixed) != 24:
        return 1
    end
    if sizeof(inferred) != 6:
        return 1
    end
    if sizeof(pointer) != sizeof(@u16):
        return 1
    end
    if parameter_size(inferred) != sizeof(@u16):
        return 1
    end

    length: i32 = 3
    runtime: u16[length]
    length = 7

    if sizeof(runtime) != 6:
        return 1
    end
    if sizeof(u16[length]) != 14:
        return 1
    end

    union_size: u64 = sizeof(u8 | u16)
    if union_size < sizeof(u16):
        return 1
    end

    if global_size != 24 or global_arithmetic != 25:
        return 1
    end
    if global_cast != 8:
        return 1
    end
    if global_large != 4294967296:
        return 1
    end
    if global_large_arithmetic != 4294967297:
        return 1
    end

    print(global_size)
    print("\n")
    print(global_arithmetic)
    print("\n")
    print(global_large)
    print("\n")
    print(global_large_arithmetic)
    print("\n")
    return 0
end
FLO

cat >"$positive_base.expected" <<'EXPECTED'
24
25
4294967296
4294967297
EXPECTED

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: sizeof operand fixture did not compile\n'
    exit 1
fi

if ! "$positive_base" >"$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: sizeof operand fixture failed at runtime\n'
    exit 1
fi

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    exit 1
fi

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$sizeof_operand_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: invalid sizeof operand accepted: %s\n' "$rejection_name"
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
        printf 'FAIL: missing sizeof operand diagnostic: %s\n' "$rejection_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: rejection occurred outside typechecking\n'
        exit 1
    fi

    if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: invalid sizeof operand reached code generation\n'
        exit 1
    fi

    printf 'PASS: %s\n' "$rejection_name"
}

expect_rejection void_type 'sizeof requires a sized type' <<'FLO'
func main(): int
    print(sizeof(void))
    return 0
end
FLO

expect_rejection incomplete_array 'sizeof requires a known array length' <<'FLO'
func main(): int
    print(sizeof(u8[]))
    return 0
end
FLO

expect_rejection incomplete_alias 'sizeof requires a known array length' <<'FLO'
type Incomplete = u8[]

func main(): int
    print(sizeof(Incomplete))
    return 0
end
FLO

expect_rejection bool_length 'sizeof array length requires an integer type' <<'FLO'
func main(): int
    length: bool = true
    print(sizeof(u8[length]))
    return 0
end
FLO

expect_rejection unknown_length 'could not resolve sizeof array length' <<'FLO'
func main(): int
    print(sizeof(u8[missing]))
    return 0
end
FLO

printf 'sizeof operand and large-size checks passed.\n'