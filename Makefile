.PHONY: help build bootstrap rebuild test clean all

SHELL ?= sh

CC ?= cc
CFLAGS ?= -std=c99
BOOTSTRAP_CFLAGS ?= $(CFLAGS)
ASAN_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

FLOWER_C := bin/Flower.c
FLOWER_BIN := bin/Flower
BACKUP_BIN := bin/Flower_backup
BOOTSTRAP_BIN := bin/Flower_bootstrap
NEW_C := bin/Flower_new.c
NEW_BIN := bin/Flower_new_bin
TEST_C := bin/Flower_test.c
OUT_BIN := output/out

help:
	@echo "Flower Compiler\t\t:   Build Commands"
	@echo "\tmake build\t:   Rebuild Flower from source using a C Compiler"
	@echo "\tmake bootstrap\t:   Self-hosting verification pass"
	@echo "\tmake rebuild\t:   Clean rebuild Flower"
	@echo "\tmake test\t:   Run test suite"
	@echo "\tmake clean\t:   Remove generated build artifacts"

build:
	$(CC) $(CFLAGS) $(FLOWER_C) -o $(FLOWER_BIN)
	./$(FLOWER_BIN) ./src/main.flo ./bin/Flower_new
	cp $(NEW_C) $(FLOWER_C)
	$(CC) $(CFLAGS) $(NEW_C) -o $(FLOWER_BIN)
	rm -f $(NEW_C) $(NEW_BIN)

bootstrap:
	CC="$(CC)" \
	CFLAGS="$(CFLAGS)" \
	BOOTSTRAP_CFLAGS="$(BOOTSTRAP_CFLAGS) $(ASAN_CFLAGS)" \
	FLOWER_C="./$(FLOWER_C)" \
	FLOWER_BIN="./$(FLOWER_BIN)" \
	BACKUP="./$(BACKUP_BIN)" \
	BOOTSTRAP_BIN="./$(BOOTSTRAP_BIN)" \
	NEW_C="./$(NEW_C)" \
	NEW_BIN="./$(NEW_BIN)" \
	TEST_C="./$(TEST_C)" \
	./scripts/build.sh
	@echo "Verified bootstrap build complete"

rebuild: clean build
	@echo "Clean rebuild complete"

test: build
	mkdir -p output
	./$(FLOWER_BIN) ./examples/test.flo ./$(OUT_BIN)
	./$(OUT_BIN)

clean:
	rm -rf output/
	rm -f $(FLOWER_BIN) $(BACKUP_BIN) $(BOOTSTRAP_BIN) $(NEW_C) $(NEW_BIN) $(TEST_C)
	@echo "Removed artifacts"

all: build test