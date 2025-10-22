CC = clang
CFLAGS = -Wall -Wextra -std=c17 -g -Iinclude
LDFLAGS =
# Apple Silicon优化
UNAME_ARCH := $(shell uname -m)
ifeq ($(UNAME_ARCH), arm64)
    CFLAGS += -mcpu=native
endif

SRCDIR = src
INCDIR = include
BINDIR = bin
OUTDIR = output

SOURCES = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BINDIR)/minic	
all: $(TARGET)

# 链接可执行文件
$(TARGET): $(SOURCES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "编译完成: $@"

# 清理
clean:
	rm -rf $(BINDIR)
	@echo "清理构建文件..."

# 运行测试（需手动创建test目录）
test: $(TARGET)
	@if [ -d "test" ]; then \
		./$(TARGET) test/test.src output; \
	else \
		echo "请先创建 test/test.src 文件"; \
	fi

.PHONY: all clean test