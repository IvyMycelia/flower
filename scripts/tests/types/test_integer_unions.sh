#!/bin/sh
set -eu

mkdir -p output
integer_union_dir=$(mktemp -d ./output/integer_unions.XXXXXX)

positive_base="$integer_union_dir/positive"

cat >"$positive_base.flo" <<'FLO'
type Byte = u8
type Choice = Byte | u16
type OptionalByte = ?Byte

global_byte: OptionalByte = 255
global_wide: string | u64 = 18446744073709551615
global_choice: Choice = 300 as u16

func accept(value: u8 | string): u8
    if value is u8:
        return value as u8
    end

    return 0
end

func make_byte(): OptionalByte
    return 250 + 5
end

func typed_byte(): u8
    return 20
end

func main(): int
    small: u8 = 20
    first: Choice = small
    fourth: Choice = 300 as u16
    maybe: OptionalByte = 20
    mixed: u8 | string = 20
    compound: OptionalByte = 250 + 5
    from_call: Choice = typed_byte()
    typed_expression: Choice = small + 1
    reversed: u16 | u8 = small
    canonical: int | string = 2147483647

    if first is u8:
        print(first as u8)
    else:
        return 1
    end
    print("\n")

    if fourth is u16:
        print(fourth as u16)
    else:
        return 1
    end
    print("\n")

    if maybe is u8:
        print(maybe as u8)
    else:
        return 1
    end
    print("\n")

    if mixed is u8:
        print(mixed as u8)
    else:
        return 1
    end
    print("\n")

    if compound is u8:
        print(compound as u8)
    else:
        return 1
    end
    print("\n")

    if from_call is u8:
        print(from_call as u8)
    else:
        return 1
    end
    print("\n")

    if typed_expression is u8:
        print(typed_expression as u8)
    else:
        return 1
    end
    print("\n")

    if reversed is u8:
        print(reversed as u8)
    else:
        return 1
    end
    print("\n")

    if canonical is i32:
        print(canonical as i32)
    else:
        return 1
    end
    print("\n")

    maybe = 42
    if maybe is u8:
        print(maybe as u8)
    else:
        return 1
    end
    print("\n")

    first = small
    if first is u8:
        print(first as u8)
    else:
        return 1
    end
    print("\n")

    print(accept(250 + 5))
    print("\n")

    returned: OptionalByte = make_byte()
    if returned is u8:
        print(returned as u8)
    else:
        return 1
    end
    print("\n")

    if global_byte is u8:
        print(global_byte as u8)
    else:
        return 1
    end
    print("\n")

    if global_wide is u64:
        print(global_wide as u64)
    else:
        return 1
    end
    print("\n")

    if global_choice is u16:
        print(global_choice as u16)
    else:
        return 1
    end
    print("\n")

    return 0
end
FLO

cat >"$positive_base.expected" <<'EXPECTED'
20
300
20
20
255
20
21
20
2147483647
42
20
255
255
255
18446744073709551615
300
EXPECTED

if ! ./bin/Flower "$positive_base.flo" "$positive_base" >"$positive_base.log" 2>&1; then
    cat "$positive_base.log"
    printf 'FAIL: valid integer_union fixture did not compile\n'
    exit 1
fi

if ! "$positive_base" >"$positive_base.actual" 2>"$positive_base.runtime.log"; then
    cat "$positive_base.runtime.log"
    printf 'FAIL: valid integer-union fixture failed at runtime\n'
    exit 1
fi

if ! cmp -s "$positive_base.expected" "$positive_base.actual"; then
    diff -u "$positive_base.expected" "$positive_base.actual" || :
    printf 'FAIL: integer_union output differs\n'
    exit 1
fi

expect_rejection() {
    rejection_name=$1
    rejection_message=$2
    rejection_base="$integer_union_dir/$rejection_name"

    cat >"$rejection_base.flo"

    if ./bin/Flower "$rejection_base.flo" "$rejection_base" >"$rejection_base.log" 2>&1; then
        printf 'FAIL: invalid integer_union fixture was accepted: %s\n' "$rejection_name"
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
            printf 'FAIL: missing integer-union diagnostics: %s\n' "$rejection_name"
            exit 1
        fi

        if ! grep -F 'Typecheck failed' "$rejection_base.log" >/dev/null; then
            cat "$rejection_base.log"
            printf 'FAIL: fixture failed outside typchecking: %s\n' "$rejection_name"
            exit 1
        fi

        if grep -F 'Codegen...' "$rejection_base.log" >/dev/null; then
            cat "$rejection_base.log"
            printf 'FAIL: invalid integer-union fixture reached code generation: %s\n' "$rejection_name"
            exit 1
        fi

        printf 'PASS: %s\n' "$rejection_name"
}

expect_rejection ambiguous_literal 'ambiguous integer-union initializer' <<'FLO'
func main(): int
    value: u8 | u16 = 20
    return 0
end
FLO

expect_rejection ambiguous_large 'ambiguous integer-union initializer' <<'FLO'
func main(): int
    value: u8 | u16 = 300
    return 0
end
FLO

expect_rejection ambiguous_expression 'ambiguous integer-union initializer' <<'FLO'
type Byte = u8
type Choice = Byte | u16

func main(): int
    value: Choice = 10 + 10
    return 0
end
FLO

expect_rejection ambiguous_reversed 'ambiguous integer-union initializer' <<'FLO'
func main(): int
    value: u16 | u8 = 300
    return 0
end
FLO

expect_rejection ambiguous_global 'ambiguous integer-union initializer' <<'FLO'
value: u8 | u16 = 20

func main(): int
    return 0
end
FLO

expect_rejection ambiguous_assignment 'ambiguous integer-union initializer' <<'FLO'
func main(): int
    value: u8 | u16 = 20 as u8
    value = 20
    return 0
end
FLO

expect_rejection ambiguous_argument 'ambiguous integer-union initializer' <<'FLO'
func accept(value: u8 | u16): int
    return 0
end

func main(): int
    return accept(20)
end
FLO

expect_rejection ambiguous_return 'ambiguous integer-union initializer' <<'FLO'
func make_value(): u8 | u16
    return 20
end

func main(): int
    return 0
end
FLO

expect_rejection nullable_range 'integer literal outside range of u8' <<'FLO'
func main(): int
    value: ?u8 = 256
    return 0
end
FLO

expect_rejection mixed_range 'integer literal outside range of u8' <<'FLO'
func main(): int
    value: string | u8 = 256
    return 0
end
FLO

expect_rejection typed_mismatch 'variable initializer does not match declared type' <<'FLO'
func main(): int
    small: i8 = 20
    value: u8 | u16 = small
    return 0
end
FLO

printf 'Integer union selection passed.\n'