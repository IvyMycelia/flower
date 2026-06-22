.PHONY: help stage0 build bootstrap rebuild test clean all

CC ?= cc
CFLAGS ?=
BOOTSTRAP_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

STAGE0_C := bin/Flower.c
STAGE0_BIN := bin/Flower_stage0
FLOWER_BIN := bin/Flower
NEW_C := bin/Flower_new.c
OUT_BIN := output/out

help:
	@echo "Flower Compiler\t\t:   Build Commands"
	@echo "\tmake stage0\t:   Build portable stage0 compiler from bin/Flower.c"
	@echo "\tmake build\t:   Rebuild Flower from source using stage0"
	@echo "\tmake bootstrap\t:   Self-hosting verification pass"
	@echo "\tmake test\t:   Run test suite"
	@echo "\tmake clean\t:   Remove generated build artifacts"

stage0: $(STAGE0_BIN)

$(STAGE0_BIN): $(STAGE0_C)
	$(CC) $(CFLAGS) $(STAGE0_C) -o $(STAGE0_BIN)

build: stage0
	./$(STAGE0_BIN) ./src/main.flo ./bin/Flower_new
	$(CC) $(CFLAGS) $(NEW_C) -o $(FLOWER_BIN)
	cp $(NEW_C) $(STAGE0_C)
	rm -f $(NEW_C)

bootstrap: stage0
	CC="$(CC)" BOOTSTRAP_CFLAGS="$(BOOTSTRAP_CFLAGS)" WORKING=./$(STAGE0_BIN) ./scripts/build.sh
	@echo "Verified bootstrap build complete"

rebuild: clean build
	@echo "Clean rebuild complete"

test: build
	mkdir -p output
	./$(FLOWER_BIN) ./examples/test.flo ./$(OUT_BIN)
	./$(OUT_BIN)

clean:
	rm -rf build/ output/
	rm -f bin/Flower bin/Flower_backup bin/Flower_new bin/Flower_new.c bin/Flower_new_bin bin/Flower_test bin/Flower_test.c bin/Flower_stage0
	@echo "Removed artifacts"

all: build test
