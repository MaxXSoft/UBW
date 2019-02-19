# external parameters
export CROSS_PREFIX = /usr/local/opt/llvm/bin/

# C compiler
CFLAGS := -c -O$(OPT_LEVEL) -target mipsel -mips32 -G8 -Wall -Werror
CFLAGS += -std=gnu99 -fno-builtin -nostdlib -nostdinc
CFLAGS += -mno-abicalls -msoft-float
export CC := $(CROSS_PREFIX)clang $(CFLAGS)

# assembler
ASFLAGS := -c -O$(OPT_LEVEL) -target mipsel -mips32 -Wall -Werror
ASFLAGS += -nostdlib -mno-abicalls -msoft-float
export AS := $(CROSS_PREFIX)clang $(ASFLAGS)

# linker
LDFLAGS := -nostdlib
LD := $(CROSS_PREFIX)ld.lld $(LDFLAGS)

# object copy
OBJCFLAGS := -O binary -j .text
OBJC := $(CROSS_PREFIX)llvm-objcopy $(OBJCFLAGS)

# object dump
OBJDFLAGS := -D
OBJD := $(CROSS_PREFIX)llvm-objdump $(OBJDFLAGS)
