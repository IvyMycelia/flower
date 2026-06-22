#!/usr/bin/env bash
set -euo pipefail

CC="${CC:-cc}"
BOOTSTRAP_CFLAGS="${BOOTSTRAP_CFLAGS:--fsanitize=address -fno-omit-frame-pointer}"

WORKING="${WORKING:-./bin/Flower}"
BACKUP="${BACKUP:-./bin/Flower_backup}"
NEW_C="${NEW_C:-./bin/Flower_new.c}"
NEW_BIN="${NEW_BIN:-./bin/Flower_new_bin}"
TEST_C="${TEST_C:-./bin/Flower_test.c}"

echo "=== Building new version ==="
"$WORKING" ./src/main.flo ./bin/Flower_new

echo "=== Compiling new binary ==="
"$CC" $BOOTSTRAP_CFLAGS "$NEW_C" -o "$NEW_BIN"

echo "=== Testing new compiler ==="
"$NEW_BIN" ./src/main.flo ./bin/Flower_test

echo "=== Idempotency check ==="
if ! diff "$NEW_C" "$TEST_C" > /dev/null 2>&1; then
    echo "ERROR: Generated code differs between runs!"
    echo "This indicates non-deterministic compilation."
    rm -f "$NEW_BIN"
    exit 1
fi

echo "=== Accepting new version ==="
cp "$WORKING" "$BACKUP"
cp "$NEW_BIN" ./bin/Flower

echo "Bootstrap successful"
echo "Previous version backed up to: $BACKUP"

cp "$NEW_C" ./bin/Flower.c
rm -f "$NEW_C" "$NEW_BIN" "$TEST_C"
