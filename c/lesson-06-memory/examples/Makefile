# Makefile for Lesson 6: Memory Management
# Compiler: gcc/clang
# Usage: make all    - Build all examples
#        make run    - Run all examples
#        make clean  - Remove all binaries

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
DEBUG_FLAGS = -g -fsanitize=address -fsanitize=leak
LDFLAGS = 

# Target executables
TARGETS = ex01-memory-basic \
          ex02-realloc \
          ex03-aligned \
          ex04-memory-errors \
          ex05-audio-buffer-pool

# Default target
all: $(TARGETS)

# Individual targets
ex01-memory-basic: ex01-memory-basic.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex02-realloc: ex02-realloc.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex03-aligned: ex03-aligned.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex04-memory-errors: ex04-memory-errors.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex05-audio-buffer-pool: ex05-audio-buffer-pool.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Debug builds with AddressSanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Run all examples
run: all
	@echo "=== Running ex01-memory-basic ==="
	@./ex01-memory-basic
	@echo ""
	@echo "=== Running ex02-realloc ==="
	@./ex02-realloc
	@echo ""
	@echo "=== Running ex03-aligned ==="
	@./ex03-aligned
	@echo ""
	@echo "=== Running ex04-memory-errors ==="
	@./ex04-memory-errors
	@echo ""
	@echo "=== Running ex05-audio-buffer-pool ==="
	@./ex05-audio-buffer-pool

# Run with valgrind
valgrind: all
	@echo "=== Running with Valgrind ==="
	valgrind --leak-check=full --show-leak-kinds=all ./ex01-memory-basic
	valgrind --leak-check=full --show-leak-kinds=all ./ex02-realloc
	valgrind --leak-check=full --show-leak-kinds=all ./ex05-audio-buffer-pool

# Run specific error demonstrations
demo-overflow: ex04-memory-errors.c
	$(CC) $(CFLAGS) -DDEMONSTRATE_OVERFLOW -fsanitize=address -g -o ex04-overflow $<
	@echo "Running buffer overflow demo..."
	./ex04-overflow

demo-uaf: ex04-memory-errors.c
	$(CC) $(CFLAGS) -DDEMONSTRATE_UAF -fsanitize=address -g -o ex04-uaf $<
	@echo "Running use-after-free demo..."
	./ex04-uaf

demo-leak: ex04-memory-errors.c
	$(CC) $(CFLAGS) -DDEMONSTRATE_LEAK -fsanitize=leak -g -o ex04-leak $<
	@echo "Running memory leak demo..."
	./ex04-leak

demo-double-free: ex04-memory-errors.c
	$(CC) $(CFLAGS) -DDEMONSTRATE_DOUBLE_FREE -fsanitize=address -g -o ex04-double-free $<
	@echo "Running double free demo..."
	./ex04-double-free

# Clean up
clean:
	rm -f $(TARGETS) ex04-overflow ex04-uaf ex04-leak ex04-double-free
	rm -f *.o *.out

# Phony targets
.PHONY: all run clean debug valgrind demo-overflow demo-uaf demo-leak demo-double-free

# Help
help:
	@echo "Available targets:"
	@echo "  all              - Build all examples"
	@echo "  run              - Run all examples"
	@echo "  clean            - Remove all binaries"
	@echo "  debug            - Build with AddressSanitizer"
	@echo "  valgrind         - Run with Valgrind memory checker"
	@echo "  demo-overflow    - Demonstrate buffer overflow"
	@echo "  demo-uaf         - Demonstrate use-after-free"
	@echo "  demo-leak        - Demonstrate memory leak"
	@echo "  demo-double-free - Demonstrate double free"
	@echo "  help             - Show this help message"
