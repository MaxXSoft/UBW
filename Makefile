# external parameters
export CROSS_PREFIX = mipsel-linux-
export OPT_LEVEL = 2

# C compiler
CFLAGS := -c -O$(OPT_LEVEL) -mips1 -EL -G8 -fno-builtin -nostdlib
CFLAGS += -nostdinc -fno-reorder-blocks -fno-reorder-functions
CFLAGS += -mno-abicalls -msoft-float -finline-functions
export CC := $(CROSS_PREFIX)gcc $(CFLAGS)

# assembler
ASFLAGS := -mips1 -EL
export AS := $(CROSS_PREFIX)as $(ASFLAGS)

# linker
LDFLAGS := -nostdlib -n -EL
LD := $(CROSS_PREFIX)ld $(LDFLAGS)

# object copy
OBJCFLAGS := -j .text -O binary
OBJC := $(CROSS_PREFIX)objcopy $(OBJCFLAGS)

# directory definitions
TARGET_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./test

# targets
TEST_TARGETS  = $(TARGET_DIR)/gpio.bin
TEST_TARGETS += $(TARGET_DIR)/memory.bin
TEST_TARGETS += $(TARGET_DIR)/vga.bin
TEST_TARGETS += $(TARGET_DIR)/uart.bin
SRC_OBJS      = $(SRC_DIR)/*.o
SRC_OBJS     += $(SRC_DIR)/library/*.o

.PHONY: all ubw test ubw_make test_make clean

all: $(TARGET_DIR) test

ubw: $(TARGET_DIR) ubw_make $(TARGET_DIR)/ubw.bin

test: $(TARGET_DIR) test_make $(TEST_TARGETS)

ubw_make:
	make -C $(SRC_DIR)
	make -C $(SRC_DIR)/library

test_make:
	make -C $(TEST_DIR)

clean:
	-rm -f $(TARGET_DIR)/*.elf
	-rm -f $(TARGET_DIR)/*.bin
	make -C $(SRC_DIR) clean
	make -C $(TEST_DIR) clean

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TARGET_DIR)/%.elf: $(TEST_DIR)/%.o
	$(LD) -Ttext 0xbfc00000 -o $@ $^

$(TARGET_DIR)/ubw.elf: linker.ld $(SRC_OBJS)
	$(LD) -T linker.ld -o $@ $(SRC_OBJS)

$(TARGET_DIR)/%.bin: $(TARGET_DIR)/%.elf
	$(OBJC) $^ $@
