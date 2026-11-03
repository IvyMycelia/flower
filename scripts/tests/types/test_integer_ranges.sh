#!/bin/sh
set -eu

mkdir -p output
range_test_dir=$(mktemp -d ./output/integer-loops.XXXXXX)

compile_and_run() {
    program_base=$1

    if ! ./bin/Flower "$program_base.flo" "$program_base" >"$program_base.compile.log" 2>&1; then
        cat "$program_base.compile.log"
        printf 'FAIL: range fixture did not compile: %s\n' "$program_base"
        exit 1
    fi

    if ! "$program_base" >"$program_base.actual" 2>"$program_base.runtime.log"; then
        cat "$program_base.runtime.log"
        printf 'FAIL: range fixture failed at runtime: %s\n' "$program_base"
        exit 1
    fi

    printf 'PASS: %s\n' "$program_base"
}

while IFS='|' read -r integer_type minimum maximum; do
    result_base="$range_test_dir/$integer_type"

    cat >"$result_base.flo" <<FLO
type Value = $integer_type

func main(): int
    low: Value = $minimum
    high: Value = $maximum
    count: i32 = 0

    for value in high - 1..=high:
        typed: Value = value
        count = count + 1
        if count > 2:
            return 1
        end
        continue
    end
    if count != 2:
        return 1
    end

    count = 0
    for value in low + 1..=low:
        typed: Value = value
        count = count + 1
        if count > 2:
            return 1
        end
        continue
    end
    if count != 2:
        return 1
    end

    count = 0
    for value in high..=high:
        count = count + 1
        if count > 1:
            return 1
        end
    end
    if count != 1:
        return 1
    end

    for value in low..low:
        return 1
    end

    count = 0
    for value in high - 1..high:
        count = count + 1
        if count > 1:
            return 1
        end
    end
    if count != 1:
        return 1
    end

    count = 0
    for value in low + 1..low:
        count = count + 1
        if count > 1:
            return 1
        end
    end
    if count != 1:
        return 1
    end

    count = 0
    for value in low..=high:
        count = count + 1
        break
    end
    if count != 1:
        return 1
    end

    count = 0
    for value in 0..=2 as Value:
        typed: Value = value
        count = count + 1
        if count > 3:
            return 1
        end
    end
    if count != 3:
        return 1
    end

    return 0
end
FLO

    compile_and_run "$result_base"
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

result_base="$range_test_dir/evaluation"

cat >"$result_base.flo" <<'FLO'
trace: i32 = 0

func start(): u8
    trace = trace * 10 + 1
    return 1
end

func finish(): u8
    trace = trace * 10 + 2
    return 3
end

func main(): int
    count: i32 = 0
    total: i32 = 0

    for value in start()..=finish():
        count = count + 1
        total = total + (value as i32)
        if count > 3:
            return 1
        end
    end

    if trace != 12 or count != 3 or total != 6:
        return 1
    end

    low: u8 = 1
    high: u8 = 3
    count = 0
    total = 0

    for value in low..=high:
        low = 200
        high = 0
        count = count + 1
        total = total + (value as i32)
        if count > 3:
            return 1
        end
    end

    if count != 3 or total != 6:
        return 1
    end

    outer: u8 = 1
    count = 0
    for outer in outer..=2:
        typed: u8 = outer
        count = count + 1
    end
    if count != 2 or outer != 1:
        return 1
    end

    count = 0
    for i in 0..2:
        for i in 0..2:
            count = count + 1
        end
    end
    if count != 4:
        return 1
    end

    __flower_range_0_from: i32 = 7
    count = 0
    for value in __flower_range_0_from..=7:
        count = count + 1
    end
    if count != 1:
        return 1
    end

    return 0
end
FLO

compile_and_run "$result_base"

while IFS='|' read -r case_name declaration from_bound to_bound diagnostic; do
    result_base="$range_test_dir/$case_name"

    cat >"$result_base.flo" <<FLO
func main(): int
    $declaration
    for value in $from_bound..=$to_bound:
        break
    end
    return 0
end
FLO

    if ./bin/Flower "$result_base.flo" "$result_base" >"$result_base.log" 2>&1; then
        printf 'FAIL: invalid range accepted: %s\n' "$case_name"
        exit 1
    else
        compiler_status=$?
    fi

    case "$compiler_status" in
        1|255) ;;
        *)
            cat "$result_base.log"
            printf 'FAIL: abnormal compiler exit %s\n' "$compiler_status"
            exit 1
            ;;
    esac

    if ! grep -F "$diagnostic" "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: missing range diagnostic: %s\n' "$case_name"
        exit 1
    fi

    if ! grep -F 'Typecheck failed' "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        exit 1
    fi

    if grep -F 'Codegen...' "$result_base.log" >/dev/null; then
        cat "$result_base.log"
        printf 'FAIL: invalid range reached code generation\n'
        exit 1
    fi

    printf 'PASS: %s\n' "$case_name"
done <<'CASES'
mixed_width|bound: u8 = 1|bound|2 as u16|range bounds must have the same integer type
mixed_sign|bound: i8 = 1|bound|2 as u8|range bounds must have the same integer type
bool_bound|bound: bool = true|bound|2|range bounds require integer types
char_bound|bound: char = 'A'|bound|2|range bounds require integer types
float_bound|bound: float = 1.0|bound|2|range bounds require integer types
contextual_range|bound: u8 = 1|bound|256|integer literal outside range of u8
default_range|unused: i32 = 0|2147483648|2147483649|integer literal outside default i32 range
unknown_bound|unused: i32 = 0|missing|2|could not resolve range bounds
CASES

printf 'Integer range-loop checks passed.\n'