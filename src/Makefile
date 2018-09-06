.PHONY: all clean

ASM_FILES := $(patsubst %.s, %.o, $(wildcard *.s))
C_FILES   := $(patsubst %.c, %.o, $(wildcard *.c))

%.o: %.c
	$(CC) -o $@ $^

%.o: %.s
	$(AS) -o $@ $^

all: $(ASM_FILES) $(C_FILES)

clean:
	-rm -f *.o
