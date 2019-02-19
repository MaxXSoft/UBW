# external parameters
export CROSS_PREFIX = mipsel-linux-

# C compiler
CFLAGS := -c -O$(OPT_LEVEL) -mips1 -EL -G8 -Wall -Werror -std=gnu99
CFLAGS += -fno-builtin -nostdlib -nostdinc -fno-reorder-blocks
CFLAGS += -fno-reorder-functions -mno-abicalls -msoft-float
CFLAGS += -finline-functions
export CC := $(CROSS_PREFIX)gcc $(CFLAGS)

# assembler
ASFLAGS := -mips32 -EL   # in order to support instructions such as 'eret'
export AS := $(CROSS_PREFIX)as $(ASFLAGS)

# linker
LDFLAGS := -nostdlib -n -EL
LD := $(CROSS_PREFIX)ld $(LDFLAGS)

# object copy
OBJCFLAGS := -O binary -j .text
OBJC := $(CROSS_PREFIX)objcopy $(OBJCFLAGS)

# object dump
OBJDFLAGS := -alD
OBJD := $(CROSS_PREFIX)objdump $(OBJDFLAGS)
