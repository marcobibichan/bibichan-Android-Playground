# Makefile for Lesson 7: String Handling
# Compiler: gcc/clang
# Usage: make all    - Build all examples
#        make run    - Run all examples
#        make clean  - Remove all binaries

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
DEBUG_FLAGS = -g -fsanitize=address
LDFLAGS = 

# Target executables
TARGETS = ex01-string-basic \
          ex02-dynamic-strings \
          ex03-string-parsing \
          ex04-safe-strings \
          ex05-audio-metadata

# Default target
all: $(TARGETS)

# Individual targets
ex01-string-basic: ex01-string-basic.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex02-dynamic-strings: ex02-dynamic-strings.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex03-string-parsing: ex03-string-parsing.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex04-safe-strings: ex04-safe-strings.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex05-audio-metadata: ex05-audio-metadata.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Debug builds with AddressSanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Run all examples
run: all
	@echo "=== Running ex01-string-basic ==="
	@./ex01-string-basic
	@echo ""
	@echo "=== Running ex02-dynamic-strings ==="
	@./ex02-dynamic-strings
	@echo ""
	@echo "=== Running ex03-string-parsing ==="
	@./ex03-string-parsing
	@echo ""
	@echo "=== Running ex04-safe-strings ==="
	@./ex04-safe-strings
	@echo ""
	@echo "=== Running ex05-audio-metadata ==="
	@./ex05-audio-metadata

# Run with valgrind
valgrind: all
	@echo "=== Running with Valgrind ==="
	valgrind --leak-check=full --show-leak-kinds=all ./ex02-dynamic-strings
	valgrind --leak-check=full --show-leak-kinds=all ./ex04-safe-strings
	valgrind --leak-check=full --show-leak-kinds=all ./ex05-audio-metadata

# Clean up
clean:
	rm -f $(TARGETS)
	rm -f *.o *.out

# Phony targets
.PHONY: all run clean debug valgrind

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build all examples"
	@echo "  run       - Run all examples"
	@echo "  clean     - Remove all binaries"
	@echo "  debug     - Build with AddressSanitizer"
	@echo "  valgrind  - Run with Valgrind memory checker"
	@echo "  help      - Show this help message"
