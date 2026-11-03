#!/bin/sh
set -eu

mkdir -p output
aggregate_arity_dir=$(mktemp -d ./output/aggregate-arity.XXXXXX)

positive_base="$aggregate_arity_dir/positive"

cat >"$positive_base.flo" <<'FLO'
struct Pair {
    left: i32,
    right: i32
}

struct Packet {
    values: u8[3],
    pair: Pair
}

type Triple = u8[3]
type PairAlias = Pair

global_values: Triple = [1, 0, 0]
global_pair: PairAlias = {4, 5}

func main(): int
    values: u8[3] = [1, 0, 0]
    inferred: i32[] = [6, 7]
    pair: Pair = {2, 3}
    packet: Packet = {[8, 9, 10], {11, 12}}
    pairs: Pair[2] = [{13, 14}, {15, 16}]

    untouched_array: u8[3]
    untouched_struct: Pair

    if values[0] != 1 or values[1] != 0 or values[2] != 0:
        return 1
    end

    if inferred[0] != 6 or inferred[1] != 7:
        return 1
    end

    if pair.left != 2 or pair.right != 3:
        return 1
    end

    if global_values[0] != 1 or global_pair.right != 5:
        return 1
    end

    print("Aggregate arity positive fixture passed.\n")
    return 0
end
FLO

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: complete aggregate fixture did not compile\n'
    exit 1
fi

if ! "$positive_base" >"$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: complete aggregate fixture failed at runtime\n'
    exit 1
fi

printf 'Aggregate arity positive fixture passed.\n' >"$positive_base.expected"

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    exit 1
fi

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$aggregate_arity_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: invalid aggregate accepted: %s\n' "$rejection_name"
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
        printf 'FAIL: missing arity diagnostic: %s\n' "$rejection_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: rejection occured outside typechecking: %s\n' "$rejection_name"
        exit 1
    fi

    if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: invalid aggregate reached code generation: %s\n' "$rejection_name"
        exit 1
    fi

    printf 'PASS: %s\n' "$rejection_name"
}

while IFS='|' read -r case_name destination initializer diagnostic; do
    expect_rejection "$case_name" "$diagnostic" <<FLO
struct Pair {
    left: i32,
    right: i32
}

struct Packet {
    values: u8[3],
    pair: Pair
}

struct UnionField {
    value: u8 | u16
}

type Triple = u8[3]
type PairAlias = Pair

func main(): int
    value: $destination = $initializer
    return 0
end
FLO
done <<'CASES'
array_missing|u8[3]|[1]|array literal: expected 3 elements, found 1
array_excess|u8[3]|[1, 2, 3, 4]|array literal: expected 3 elements, found 4
array_empty|u8[3]|[]|array literal: expected 3 elements, found 0
array_alias|Triple|[1, 2]|array literal: expected 3 elements, found 2
struct_missing|Pair|{1}|struct literal: expected 2 fields, found 1
struct_excess|Pair|{1, 2, 3}|struct literal: expected 2 fields, found 3
struct_empty|Pair|{}|struct literal: expected 2 fields, found 0
struct_alias|PairAlias|{1}|struct literal: expected 2 fields, found 1
nested_array|Packet|{[1], {2, 3}}|array literal: expected 3 elements, found 1
nested_struct|Packet|{[1, 2, 3], {4}}|struct literal: expected 2 fields, found 1
array_of_structs|Pair[2]|[{1, 2}, {3}]|struct literal: expected 2 fields, found 1
missing_union_field|UnionField|{}|struct literal: expected 1 fields, found 0
CASES

expect_rejection global_array 'array literal: expected 3 elements, found 1' <<'FLO'
value: u8[3] = [1]

func main(): int
    return 0
end
FLO

expect_rejection argument_struct 'struct literal: expected 2 fields, found 1' <<'FLO'
struct Pair {
    left: i32,
    right: i32
}

func accept(value: Pair): int
    return 0
end

func main(): int
    return accept({1})
end
FLO

expect_rejection return_struct 'struct literal: expected 2 fields, found 1' <<'FLO'
struct Pair {
    left: i32,
    right: i32
}

func make_pair(): Pair
    return {1}
end

func main(): int
    return 0
end
FLO

printf 'Aggregate literal arity checks passed.\n'