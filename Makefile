# external parameters
export CROSS_PREFIX = mipsel-linux-
export OPT_LEVEL = 2

# C compiler
CFLAGS := -O$(OPT_LEVEL) -fno-builtin -nostdlib -nostdinc -mips1
CFLAGS += -fno-reorder-blocks -fno-reorder-functions -mno-float
export CC := $(CROSS_PREFIX)gcc $(CFLAGS)

# assembler
ASFLAGS := -mips1
export AS := $(CROSS_PREFIX)as $(ASFLAGS)

# linker
LDFLAGS := -nostdlib
export LD := $(CROSS_PREFIX)ld $(LDFLAGS)

# directory definitions
TARGET_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./test


.PHONY: all test clean

all: $(TARGET_DIR) test

test: $(TARGET_DIR)
	make -C $(TEST_DIR)

clean:
	-rm -rf $(TARGET_DIR)
	make -C $(TEST_DIR) clean

$(TARGET_DIR):
	mkdir $(TARGET_DIR)
