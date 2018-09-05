%.o: %.s
	$(AS) -o $@ $^

all: $(patsubst %.s, %.o, $(wildcard *.s))

clean:
	-rm -rf *.o
