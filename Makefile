export OPT_LEVEL = 3

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	include make/macos.make
else
	include make/debian.make
endif

# directory definitions
TARGET_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./test

# targets
TEST_OBJS_TXT    = $(wildcard $(TEST_DIR)/*.o)
TEST_TARGETS_TXT = $(patsubst %.o, $(TARGET_DIR)/%.bin, $(notdir $(TEST_OBJS_TXT)))
SRC_OBJS_TXT     = $(wildcard $(SRC_DIR)/*.o $(SRC_DIR)/library/*.o)

.PHONY: all ubw test ubw_make test_make test_targets clean

all: $(TARGET_DIR) ubw test

ubw: $(TARGET_DIR) ubw_make $(TARGET_DIR)/ubw.bin

test: $(TARGET_DIR) test_make test_targets

ubw_make:
	make -C $(SRC_DIR)

test_make:
	make -C $(TEST_DIR)

test_targets:
	$(eval TEST_TARGETS = $(TEST_TARGETS_TXT))
	$(MAKE) $(TEST_TARGETS)

clean:
	-rm -f $(TARGET_DIR)/*.elf
	-rm -f $(TARGET_DIR)/*.elf.s
	-rm -f $(TARGET_DIR)/*.bin
	make -C $(SRC_DIR) clean
	make -C $(TEST_DIR) clean

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TARGET_DIR)/%.elf: $(TEST_DIR)/%.o
	$(LD) -Ttext 0xbfc00000 -o $@ $^

$(TARGET_DIR)/ubw.elf: linker.ld
	$(eval SRC_OBJS = $(SRC_OBJS_TXT))
	$(LD) -T linker.ld -o $@ $(SRC_OBJS)
	$(OBJD) $@ > $@.s

$(TARGET_DIR)/%.bin: $(TARGET_DIR)/%.elf
	$(OBJC) -j .data $^ $@
