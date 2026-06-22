#!/bin/sh
set -eu

CC="${CC:-cc}"
CFLAGS="${CFLAGS:--std=c99}"
BOOTSTRAP_CFLAGS="${BOOTSTRAP_CFLAGS:--$CFLAGS}"

FLOWER_C="${FLOWER_C:-./bin/Flower.c}"
FLOWER_BIN="${FLOWER_BIN:-./bin/Flower}"
BACKUP="${BACKUP:-./bin/Flower_backup}"
BOOTSTRAP_BIN="${BOOTSTRAP_BIN:-./bin/Flower_bootstrap}"
NEW_C="${NEW_C:-./bin/Flower_new.c}"
NEW_BIN="${NEW_BIN:-./bin/Flower_new_bin}"
TEST_C="${TEST_C:-./bin/Flower_test.c}"

echo "=== Building bootstrap compiler ==="
"$CC" $CFLAGS "$FLOWER_C" -o "$BOOTSTRAP_BIN"

echo "=== Building new version ==="
"$BOOTSTRAP_BIN" ./src/main.flo ./bin/Flower_new

echo "=== Compiling new binary ==="
"$CC" $BOOTSTRAP_CFLAGS "$NEW_C" -o "$NEW_BIN"

echo "=== Testing new compiler ==="
"$NEW_BIN" ./src/main.flo ./bin/Flower_test

echo "=== Idempotency check ==="
if ! cmp -s "$NEW_C" "$TEST_C"; then
    echo "ERROR: Generated code differs between runs!"
    echo "This indicates non-deterministic compilation."
    rm -f "$BOOTSTRAP_BIN" "$NEW_BIN"
    exit 1
fi

echo "=== Accepting new version ==="
if [ -f "$FLOWER_BIN" ]; then
    cp "$FLOWER_BIN" "$BACKUP"
fi

cp "$NEW_BIN" "$FLOWER_BIN"

echo "Bootstrap successful"
echo "Previous version backed up to: $BACKUP"

cp "$NEW_C" "$FLOWER_BIN"
rm -f "$BOOTSTRAP_BIN" "$NEW_C" "$NEW_BIN" "$TEST_C"