# Makefile for lesson-03-control
# Usage: make all    - Build all examples
#        make run    - Build and run all examples
#        make clean  - Remove all binaries

CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

TARGETS = if_basic switch_advanced loops_audio goto_error benchmark_control

all: $(TARGETS)

if_basic: ex01-if-basic.c
	$(CC) $(CFLAGS) -o $@ $<

switch_advanced: ex02-switch-advanced.c
	$(CC) $(CFLAGS) -o $@ $<

loops_audio: ex03-loops-audio.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

goto_error: ex04-goto-error.c
	$(CC) $(CFLAGS) -o $@ $<

benchmark_control: ex05-benchmark-control.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

run: all
	@echo "=== Running all demos ==="
	@echo ""
	@echo "--- if_basic ---"
	./if_basic
	@echo ""
	@echo "--- switch_advanced ---"
	./switch_advanced
	@echo ""
	@echo "--- loops_audio ---"
	./loops_audio
	@echo ""
	@echo "--- goto_error ---"
	./goto_error
	@echo ""
	@echo "--- benchmark_control ---"
	./benchmark_control

.PHONY: all clean run
