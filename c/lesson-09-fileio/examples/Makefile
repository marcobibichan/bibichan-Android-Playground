# Makefile for lesson-09-fileio examples
# Purpose: Compile all file I/O examples
# Usage: make all     - compile all examples
#        make clean   - remove all binaries
#        make run     - run basic examples

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lm

# Target definitions
TARGETS = ex01-file-basic \
          ex02-wav-header \
          ex03-large-file \
          ex04-playlist-file \
          ex05-file-copy

# Source files
SOURCES = ex01-file-basic.c \
          ex02-wav-header.c \
          ex03-large-file.c \
          ex04-playlist-file.c \
          ex05-file-copy.c

# Default target
.PHONY: all
all: $(TARGETS)

# Pattern rule for compiling examples
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Individual targets with explicit dependencies
ex01-file-basic: ex01-file-basic.c
	$(CC) $(CFLAGS) -o $@ $<

ex02-wav-header: ex02-wav-header.c
	$(CC) $(CFLAGS) -o $@ $<

ex03-large-file: ex03-large-file.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ex04-playlist-file: ex04-playlist-file.c
	$(CC) $(CFLAGS) -o $@ $<

ex05-file-copy: ex05-file-copy.c
	$(CC) $(CFLAGS) -o $@ $<

# Run basic examples (no file dependencies)
.PHONY: run
run: ex01-file-basic ex04-playlist-file
	@echo "=== Running ex01-file-basic ==="
	@./ex01-file-basic
	@echo ""
	@echo "=== Running ex04-playlist-file ==="
	@./ex04-playlist-file

# Run WAV header example (requires WAV file)
.PHONY: run-wav
run-wav: ex02-wav-header
	@echo "Usage: ./ex02-wav-header <wav_file>"
	@echo "Example: ./ex02-wav-header audio.wav"

# Run large file example (requires PCM file)
.PHONY: run-large
run-large: ex03-large-file
	@echo "Usage: ./ex03-large-file <pcm_file> <bit_depth:16|24>"
	@echo "Example: ./ex03-large-file audio.pcm 24"

# Run file copy example
.PHONY: run-copy
run-copy: ex05-file-copy
	@echo "Usage: ./ex05-file-copy <source> <destination>"
	@echo "Example: ./ex05-file-copy audio.wav backup.wav"

# Clean all binaries
.PHONY: clean
clean:
	rm -f $(TARGETS) *.o *.txt *.bin *.m3u

# Create test WAV file (simple sine wave)
.PHONY: test-wav
test-wav:
	@echo "Creating test WAV file..."
	@echo "Note: This requires ffmpeg or sox to generate a test file"
	@echo "Example: ffmpeg -f lavfi -i 'sine=frequency=440:duration=5' -ar 192000 -ac 2 -c:a pcm_s24le test_192k.wav"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all       - Compile all examples"
	@echo "  clean     - Remove all binaries and generated files"
	@echo "  run       - Run basic examples (no file dependencies)"
	@echo "  run-wav   - Show usage for WAV header example"
	@echo "  run-large - Show usage for large file example"
	@echo "  run-copy  - Show usage for file copy example"
	@echo "  test-wav  - Show how to create test WAV file"
	@echo "  help      - Show this help message"
