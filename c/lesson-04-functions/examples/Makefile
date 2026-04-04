# Makefile for lesson-04-functions
# Purpose: Build all examples in this lesson
# Usage: make all     - Build all examples
#        make run     - Build and run all examples
#        make clean   - Remove all binaries

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lm

# Target executables
TARGETS = ex01-function-basic \
          ex02-parameter-passing \
          ex03-scope-lifetime \
          ex04-recursion-inline \
          ex05-function-pointers

# Default target
all: $(TARGETS)

# Individual build rules
ex01-function-basic: ex01-function-basic.c
	$(CC) $(CFLAGS) -o $@ $<

ex02-parameter-passing: ex02-parameter-passing.c
	$(CC) $(CFLAGS) -o $@ $<

ex03-scope-lifetime: ex03-scope-lifetime.c
	$(CC) $(CFLAGS) -o $@ $<

ex04-recursion-inline: ex04-recursion-inline.c
	$(CC) $(CFLAGS) -o $@ $<

ex05-function-pointers: ex05-function-pointers.c
	$(CC) $(CFLAGS) -o $@ $<

# Run all examples
run: all
	@echo "\n=== Running ex01-function-basic ==="
	@./ex01-function-basic
	@echo "\n=== Running ex02-parameter-passing ==="
	@./ex02-parameter-passing
	@echo "\n=== Running ex03-scope-lifetime ==="
	@./ex03-scope-lifetime
	@echo "\n=== Running ex04-recursion-inline ==="
	@./ex04-recursion-inline
	@echo "\n=== Running ex05-function-pointers ==="
	@./ex05-function-pointers

# Clean all binaries
clean:
	rm -f $(TARGETS)

# Phony targets
.PHONY: all run clean
