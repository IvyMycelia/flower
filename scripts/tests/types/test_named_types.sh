#!/bin/sh
set -eu

mkdir -p output
named_types_dir=$(mktemp -d ./output/named-types.XXXXXX)

cat >"$named_types_dir/recursive.flo" <<'FLO'
forward struct Node
forward struct Left
forward struct Right

struct Node {
    value: i32,
    next: @Node
}

struct Left {
    value: i32,
    right: @Right
}

struct Right {
    value: i32,
    left: @Left
}

union Storage {
    number: i32,
    node: @Node
}

type NodeAlias = Node

prop struct Holder {
    node: @NodeAlias
}

func main(): int
    first: Node
    second: Node
    first.value = 10
    first.next = &second
    second.value = 20
    second.next = null

    if first.next.value != 20:
        return 1
    end

    left: Left
    right: Right
    left.value = 30
    right.value = 40
    left.right = &right
    right.left = &left

    if left.right.value != 40:
        return 2
    end

    if right.left.value != 30:
        return 3
    end

    holder: Holder
    holder.node = &first
    if holder.node.value != 10:
        return 4
    end

    storage: Storage
    storage.number = 50
    if storage.number != 50:
        return 5
    end

    return 0
end
FLO

if ! ./bin/Flower "$named_types_dir/recursive.flo" \
    "$named_types_dir/recursive" >"$named_types_dir/compile.log" 2>&1; then
    cat "$named_types_dir/compile.log"
    printf 'FAIL: named-type fixture did not compile\n'
    exit 1
fi

if "$named_types_dir/recursive"; then
    printf 'Named-type registration checks passed.\n'
else
    result_status=$?
    printf 'FAIL: named-type fixture returned %s\n' "$result_status"
    exit 1
fi