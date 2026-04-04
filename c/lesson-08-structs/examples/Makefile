# Makefile for lesson-08-structs examples
# Purpose: Compile all structure and union examples
# Usage: make all     - compile all examples
#        make clean   - remove all binaries
#        make run     - run all examples

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = 

# Target definitions
TARGETS = ex01-struct-basic \
          ex02-struct-pointers \
          ex03-unions \
          ex04-bitfields-padding \
          ex05-audio-config

# Source files
SOURCES = ex01-struct-basic.c \
          ex02-struct-pointers.c \
          ex03-unions.c \
          ex04-bitfields-padding.c \
          ex05-audio-config.c

# Default target
.PHONY: all
all: $(TARGETS)

# Pattern rule for compiling examples
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Individual targets with explicit dependencies
ex01-struct-basic: ex01-struct-basic.c
	$(CC) $(CFLAGS) -o $@ $<

ex02-struct-pointers: ex02-struct-pointers.c
	$(CC) $(CFLAGS) -o $@ $<

ex03-unions: ex03-unions.c
	$(CC) $(CFLAGS) -o $@ $<

ex04-bitfields-padding: ex04-bitfields-padding.c
	$(CC) $(CFLAGS) -o $@ $<

ex05-audio-config: ex05-audio-config.c
	$(CC) $(CFLAGS) -o $@ $<

# Run all examples
.PHONY: run
run: all
	@echo "=== Running ex01-struct-basic ==="
	@./ex01-struct-basic
	@echo ""
	@echo "=== Running ex02-struct-pointers ==="
	@./ex02-struct-pointers
	@echo ""
	@echo "=== Running ex03-unions ==="
	@./ex03-unions
	@echo ""
	@echo "=== Running ex04-bitfields-padding ==="
	@./ex04-bitfields-padding
	@echo ""
	@echo "=== Running ex05-audio-config ==="
	@./ex05-audio-config

# Clean all binaries
.PHONY: clean
clean:
	rm -f $(TARGETS) *.o

# Show structure sizes
.PHONY: sizes
sizes: all
	@echo "=== Structure Size Analysis ==="
	@echo "Compiling with -fdump-tree-all for analysis..."

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all    - Compile all examples"
	@echo "  clean  - Remove all binaries"
	@echo "  run    - Run all examples"
	@echo "  sizes  - Show structure size analysis"
	@echo "  help   - Show this help message"
