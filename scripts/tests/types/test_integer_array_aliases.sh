#!/bin/sh
set -eu

mkdir -p output
array_alias_dir=$(mktemp -d ./output/integer-array-aliases.XXXXXX)

while IFS='|' read -r integer_type minimum maximum; do
    result_base="$array_alias_dir/$integer_type"

    cat >"$result_base.flo" <<FLO
type Element = $integer_type
type Pair = Element[2]
type PairAlias = Pair

struct Box {
    items: PairAlias
}

union Storage {
    items: PairAlias
}

forward func read_item(values: PairAlias, index: u8): Element

global_box: Box = {[$minimum, $maximum]}

func read_item(values: PairAlias, index: u8): Element
    return values[index]
end

func write_item(values: PairAlias, index: u8, value: Element): void
    values[index] = value
end

func main(): int
    local: PairAlias = [$minimum, $maximum]
    box: Box = {[$minimum, $maximum]}
    storage: Storage

    storage.items[0] = $minimum
    storage.items[1] = $maximum

    if read_item(local, 0) != $minimum:
        return 1
    end
    if read_item(local, 1) != $maximum:
        return 1
    end

    if read_item(box.items, 0) != $minimum:
        return 1
    end
    if read_item(box.items, 1) != $maximum:
        return 1
    end

    if read_item(global_box.items, 0) != $minimum:
        return 1
    end
    if read_item(global_box.items, 1) != $maximum:
        return 1
    end

    if read_item(storage.items, 0) != $minimum:
        return 1
    end
    if read_item(storage.items, 1) != $maximum:
        return 1
    end

    write_item(local, 0, 20)
    write_item(box.items, 1, 21)
    write_item(storage.items, 0, 22)

    if local[0] != 20 or box.items[1] != 21 or storage.items[0] != 22:
        return 1
    end

    print("Integer array aliases passed.\n")
    return 0
end
FLO

    if ! ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
        cat "$result_base.log"
        printf 'FAIL: array-alias fixture did not compile: %s\n' "$integer_type"
        exit 1
    fi

    if ! "$result_base" >"$result_base.actual" 2>"$result_base.runtime.log"; then
        cat "$result_base.runtime.log"
        printf 'FAIL: array-alias fixture failed at runtime: %s\n' "$integer_type"
        exit 1
    fi

    printf 'Integer array aliases passed.\n' >"$result_base.expected"

    if ! cmp -s "$result_base.expected" "$result_base.actual"; then
        diff -u "$result_base.expected" "$result_base.actual" || :
        exit 1
    fi

    printf 'PASS: %s array aliases\n' "$integer_type"
done <<'TYPES'
i8|-128|127
u8|0|255
i16|-32768|32767
u16|0|65535
i32|-2147483648|2147483647
u32|0|4294967295
i64|-9223372036854775808|9223372036854775807
u64|0|18446744073709551615
TYPES

printf 'Integer array-alias declaration checks passed.\n'