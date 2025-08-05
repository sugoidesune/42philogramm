# Minimal Makefile for visualize.c

# Configurable vars
CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -pedantic
LDFLAGS ?=
TARGET ?= philogramm
SRC := philogramm.c helpmsg.c print_fork_log.c

.PHONY: all build run clean format help

all: build
build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: build
	@./$(TARGET)

clean:
	@rm -f $(TARGET)

# Format using clang-format if available
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(SRC); \
	else \
		echo "clang-format not found."; \
	fi

help:
	@echo "Targets:"
	@echo "  build (default) - build $(TARGET)"
	@echo "  run             - build and run ./$(TARGET)"
	@echo "  clean           - remove binary"
	@echo "  format          - clang-format the source"
