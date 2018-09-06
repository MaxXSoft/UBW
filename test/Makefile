.PHONY: all clean

%.o: %.s
	$(AS) -o $@ $^

all: $(patsubst %.s, %.o, $(wildcard *.s))

clean:
	-rm -f *.o
