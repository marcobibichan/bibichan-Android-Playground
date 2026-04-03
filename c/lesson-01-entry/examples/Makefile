# File: Makefile
# Purpose: Build automation for lesson 01 examples

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

# Source files
SOURCES = hello_basic.c hello_args.c compilation_phases.c preprocessor_demo.c ndk_audio_info.c
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=)

# Default target
all: $(TARGETS)

# Pattern rule for compiling C files
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Debug build
debug: CFLAGS += -DDEBUG -g
debug: clean all

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGETS) *.i *.s

# Preprocess only
preprocess: hello_basic.i

hello_basic.i: hello_basic.c
	$(CC) -E $< -o $@

# Assembly output
assembly: hello_basic.s

hello_basic.s: hello_basic.c
	$(CC) -S $< -o $@

# Phony targets
.PHONY: all clean debug preprocess assembly

# Run all programs
run: all
	@echo "=== Running hello_basic ==="
	./hello_basic
	@echo "=== Running hello_args ==="
	./hello_args World
	@echo "=== Running ndk_audio_info ==="
	./ndk_audio_info
