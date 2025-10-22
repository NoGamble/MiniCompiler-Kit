CC = clang
CFLAGS = -Wall -Wextra -std=c17 -g -Iinclude
LDFLAGS =

# Apple Silicon optimization: NoGamble is on mac M2
UNAME_ARCH := $(shell uname -m)
ifeq ($(UNAME_ARCH), arm64)
    CFLAGS += -mcpu=native
endif

# DIR config
SRCDIR = src
INCDIR = include
BINDIR = bin
OUTDIR = output
TESTDIR = test


# File list
SOURCES = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BINDIR)/minic.o
all: $(TARGET)

# Build targets
$(TARGET): $(SOURCES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build completed: $@"

# Clean build artifacts
clean:
	rm -rf $(BINDIR)/*
	@echo "Cleaned buiid files: "
	rm -rf $(OUTDIR)/*
	@echo "Cleaned output files: "

# test
test: $(TARGET)
	@if [ -d "test" ]; then \
		./$(TARGET) test/test.src output; \
	else \
		echo "Please create test/test.src file first"; \
	fi

.PHONY: all clean test