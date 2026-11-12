#!/bin/sh
set -eu

mkdir -p output
type_bridge_dir=$(mktemp -d ./output/type-bridge.XXXXXX)

cat >"$type_bridge_dir/bridge.flo" <<'FLO'
import "src/token.flo"
import "src/ast.flo"
import "src/type_store.flo" as types
import "src/type_bridge.flo" as bridge

func main(): int
    types.reset()

    legacy: @TypeInfo = new TypeInfo
    legacy.base = TOKEN_U8
    legacy.pointer_depth = 0
    legacy.array_size = 0
    legacy.arr_size_expr = null
    legacy.name_start = 0
    legacy.name_length = 0
    legacy.name_src = null
    legacy.is_nullable = false
    legacy.is_union = false
    legacy.union_count = 0

    byte_id: int = types.primitive_from_token(TOKEN_U8)
    if bridge.builtin_type_id(legacy) != byte_id:
        return 1
    end

    legacy.pointer_depth = 2
    two_pointers: int = types.pointer_type(types.pointer_type(byte_id))
    if bridge.builtin_type_id(legacy) != two_pointers:
        return 2
    end

    legacy.array_size = 4
    array_of_pointers: int = types.array_type(two_pointers, 4)
    if bridge.builtin_type_id(legacy) != array_of_pointers:
        return 3
    end

    pointer_to_array: int = types.pointer_type(types.array_type(byte_id, 4))
    if bridge.builtin_type_id(legacy) == pointer_to_array:
        return 4
    end

    atom: @TypeAtom = new TypeAtom
    type_atom_from_info(atom, legacy)
    if bridge.builtin_atom_id(atom) != array_of_pointers:
        return 5
    end

    legacy.array_size = -1
    if bridge.builtin_type_id(legacy) != 0:
        return 6
    end

    legacy.array_size = 0
    marker: @AST = new AST
    legacy.arr_size_expr = marker
    if bridge.builtin_type_id(legacy) != 0:
        return 7
    end
    legacy.arr_size_expr = null
    prune marker

    legacy.is_nullable = true
    if bridge.builtin_type_id(legacy) != 0:
        return 8
    end
    legacy.is_nullable = false

    legacy.is_union = true
    if bridge.builtin_type_id(legacy) != 0:
        return 9
    end
    legacy.is_union = false

    legacy.base = TOKEN_IDENTIFIER
    if bridge.builtin_type_id(legacy) != 0:
        return 10
    end

    legacy.base = TOKEN_INT
    legacy.pointer_depth = 0
    if bridge.builtin_type_id(legacy) != types.primitive_from_token(TOKEN_I32):
        return 11
    end

    legacy.base = TOKEN_U8
    legacy.pointer_depth = 2
    legacy.array_size = 4
    if bridge.builtin_type_id(legacy) != array_of_pointers:
        return 12
    end

    prune atom
    prune legacy
    types.reset()
    return 0
end
FLO

if ! ./bin/Flower "$type_bridge_dir/bridge.flo" \
    "$type_bridge_dir/bridge" >"$type_bridge_dir/compile.log" 2>&1; then
    cat "$type_bridge_dir/compile.log"
    printf 'FAIL: type bridge fixture did not compile\n'
    exit 1
fi

if "$type_bridge_dir/bridge"; then
    printf 'Legacy-to-structural type bridge checks passed.\n'
else
    result_status=$?
    printf 'FAIL: type bridge check returned %s\n' "$result_status"
    exit 1
fi