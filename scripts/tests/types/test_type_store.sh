#!/bin/sh
set -eu

mkdir -p output
type_store_dir=$(mktemp -d ./output/type-store.XXXXXX)

cat >"$type_store_dir/identity.flo" <<'FLO'
import "src/token.flo"
import "src/type_store.flo" as types

func main(): int
    types.reset()

    signed_byte: int = types.primitive_from_token(TOKEN_I8)
    unsigned_byte: int = types.primitive_from_token(TOKEN_U8)
    signed_word: int = types.primitive_from_token(TOKEN_I32)

    if signed_byte == 0 or unsigned_byte == 0 or signed_word == 0:
        return 1
    end

    if signed_byte == unsigned_byte:
        return 2
    end

    if types.primitive_from_token(TOKEN_INT) != signed_word:
        return 3
    end

    if types.primitive_from_token(TOKEN_I8) != signed_byte:
        return 4
    end

    byte_pointer: int = types.pointer_type(unsigned_byte)
    byte_pair: int = types.array_type(unsigned_byte, 2)

    pointer_to_array: int = types.pointer_type(byte_pair)
    array_of_pointers: int = types.array_type(byte_pointer, 2)

    if pointer_to_array == array_of_pointers:
        return 5
    end

    if types.pointer_type(types.array_type(unsigned_byte, 2)) != pointer_to_array:
        return 6
    end

    if types.array_type(unsigned_byte, 3) == byte_pair:
        return 7
    end

    if types.array_type(signed_byte, 2) == byte_pair:
        return 8
    end

    empty: int = types.array_type(unsigned_byte, 0)
    if empty == byte_pair:
        return 9
    end

    nested: int = types.array_type(byte_pair, 3)
    flat: int = types.array_type(unsigned_byte, 6)
    if nested == flat:
        return 10
    end

    for i in 1..128:
        generated: int = types.array_type(unsigned_byte, i as u64)
        if generated == 0:
            return 11
        end
    end

    if types.pointer_type(byte_pair) != pointer_to_array:
        return 12
    end

    members: int[2] = [signed_byte, unsigned_byte]
    original_union: int = types.union_type(&members[0], 2)

    if types.union_type(&members[0], 2) != original_union:
        return 14
    end

    members[0] = unsigned_byte
    members[1] = signed_byte
    reversed_union: int = types.union_type(&members[0], 2)

    if reversed_union == original_union:
        return 15
    end

    members[0] = signed_byte
    members[1] = unsigned_byte
    if types.union_type(&members[0], 2) != original_union:
        return 16
    end

    if types.union_type(&members[0], 1) == original_union:
        return 17
    end

    members[1] = signed_word
    if types.union_type(&members[0], 2) == original_union:
        return 18
    end

    nullable_members: int[2] = [unsigned_byte, types.primitive_from_token(TOKEN_NULL)]
    nullable_union: int = types.union_type(&nullable_members[0], 2)

    if nullable_union == original_union:
        return 19
    end

    if types.pointer_type(original_union) == original_union:
        return 20
    end

    first_struct: int = types.named_type(1, false)
    second_struct: int = types.named_type(2, false)
    raw_union: int = types.named_type(1, true)

    if types.named_type(1, false) != first_struct:
        return 21
    end

    if first_struct == second_struct:
        return 22
    end

    if first_struct == raw_union:
        return 23
    end

    first_pointer: int = types.pointer_type(first_struct)
    second_pointer: int = types.pointer_type(second_struct)

    if first_pointer == second_pointer:
        return 24
    end

    if types.pointer_type(types.named_type(1, false)) != first_pointer:
        return 25
    end

    if types.array_type(first_struct, 2) == types.array_type(second_struct, 2):
        return 26
    end

    types.reset()

    rebuilt: int = types.primitive_from_token(TOKEN_U8)
    if rebuilt != 1:
        return 13
    end

    types.reset()
    return 0
end
FLO

if ! ./bin/Flower "$type_store_dir/identity.flo" \
    "$type_store_dir/identity" >"$type_store_dir/compile.log" 2>&1; then
    cat "$type_store_dir/compile.log"
    printf 'FAIL: type store fixture did not compile\n'
    exit 1
fi

if "$type_store_dir/identity"; then
    printf 'Structural type identity checks passed.\n'
else
    result_status=$?
    printf 'FAIL: structural type identity check returned %s\n' "$result_status"
    exit 1
fi