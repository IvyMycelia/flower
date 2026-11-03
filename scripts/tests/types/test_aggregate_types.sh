#!/bin/sh
set -eu

mkdir -p output
aggregate_types_dir=$(mktemp -d ./output/aggregate-types.XXXXXX)
positive_base="$aggregate_types_dir/positive"

cat >"$positive_base.flo" <<'FLO'
type Byte = u8

struct Bounds {
    low: i8,
    high: u64
}

struct Choices{
    selected: u8 | u16,
    optional: ?u8,
    mixed: u8 | string
}

struct Packet {
    bytes: u8[3],
    bounds: Bounds
}

global_bounds: Bounds = {-128, 18446744073709551615}

func accept(value: Bounds): bool
    return value.low == -128 and value.high == 18446744073709551615
end

func make_bounds(): Bounds
    return {-128, 18446744073709551615}
end

func main(): int
    bytes: Byte[3] = [0, 255, 250 + 5]
    wide: u64[2] = [0, 18446744073709551615]
    bounds: Bounds = {-128, 18446744073709551615}
    packet: Packet = {[0, 1, 255], {-128, 18446744073709551615}}
    pairs: Bounds[2] = [{-128, 0}, {127, 18446744073709551615}]

    small: u8 = 20
    choices: Choices = {small, 250 + 5, 20}
    explicit_choice: Choices = {300 as u16, null, "text"}

    if bytes[0] != 0 or bytes[1] != 255 or bytes[2] != 255:
        return 1
    end

    if wide[1] != 18446744073709551615:
        return 1
    end

    if not accept(bounds):
        return 1
    end

    if not accept({-128, 18446744073709551615}):
        return 1
    end

    returned: Bounds = make_bounds()
    if not accept(returned):
        return 1
    end

    bounds = {-128, 18446744073709551615}
    if not accept(bounds) or not accept(global_bounds):
        return 1
    end

    if packet.bytes[2] != 255 or packet.bounds.low != -128:
        return 1
    end

    if pairs[0].low != -128 or pairs[1].high != 18446744073709551615:
        return 1
    end

    if choices.selected is u8:
        value: u8 = choices.selected as u8
        if value != 20:
            return 1
        end
    else:
        return 1
    end

    if choices.optional is u8:
        value: u8 = choices.optional as u8
        if value != 255:
            return 1
        end
    else:
        return 1
    end

    if choices.mixed is u8:
        value: u8 = choices.mixed as u8
        if value != 20:
            return 1
        end
    else:
        return 1
    end

    if explicit_choice.selected is u16:
        value: u16 = explicit_choice.selected as u16
        if value != 300:
            return 1
        end
    else:
        return 1
    end

    if explicit_choice.optional != null:
        return 1
    end

    if explicit_choice.mixed is string:
        value: string = explicit_choice.mixed as string
        if value != "text":
            return 1
        end
    else:
        return 1
    end

    print("Aggregate element typing passed.\n")
    return 0
end
FLO

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: aggregate typing fixture did not compile\n'
    exit 1
fi

if ! "$positive_base" > "$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: aggregate typing fixture failed at runtime\n'
    exit 1
fi

printf 'Aggregate element typing passed.\n' >"$positive_base.expected"

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    exit 1
fi

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$aggregate_types_dir/$rejection_name"

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
            printf 'FAIL: abnormal compiler exit: %s: %s\n' "$rejection_status" "$rejection_name"
            exit 1
            ;;
    esac

    if ! grep -F "$rejection_message" "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: missing aggregate diagnostic: %s\n' "$rejection_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
        cat "$rejection_base.log"
        printf 'FAIL: rejection occurred outside typechecking: %s\n' "$rejection_name"
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
struct ByteBox {
    value: u8
}

struct OptionalBox {
    value: ?u8
}

struct ChoiceBox {
    value: u8 | u16
}

struct Nested {
    values: u8[2]
}

func main(): int
    typed: u16 = 20
    value: $destination = $initializer
    return 0
end
FLO
done <<'CASES'
array_range|u8[1]|[256]|integer literal outside range of u8
array_negative|u8[1]|[-1]|integer literal cannot initialize u8
signed_range|i8[1]|[-129]|integer literal outside range of i8
array_typed_mismatch|u8[1]|[typed]|aggregate element does not match destination type
struct_range|ByteBox|{256}|integer literal outside range of u8
struct_typed_mismatch|ByteBox|{typed}|aggregate element does not match destination type
nullable_range|OptionalBox|{256}|integer literal outside range of u8
ambiguous_literal|ChoiceBox|{20}|ambiguous integer-union initializer
ambiguous_magnitude|ChoiceBox|{300}|ambiguous integer-union initializer
ambiguous_expression|ChoiceBox|{10 + 10}|ambiguous integer-union initializer
nested_range|Nested|{[0, 256]}|integer literal outside range of u8
CASES

expect_rejection global_range 'integer literal outside range of u8' <<'FLO'
values: u8[1] = [256]

func main(): int
    return 0
end
FLO

expect_rejection argument_range 'integer literal outside range of u8' <<'FLO'
struct ByteBox {
    value: u8
}

func accept(value: ByteBox): int
    return 0
end

func main(): int
    return accept({256})
end
FLO

expect_rejection return_range 'integer literal outside range of u8' <<'FLO'
struct ByteBox {
    value: u8
}

func make_box(): ByteBox
    return {256}
end

func main(): int
    return 0
end
FLO

printf 'Aggregate element and union initialization checks passed.\n'