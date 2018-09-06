.PHONY: all clean

%.o: %.c
	$(CC) -o $@ $^

all: $(patsubst %.c, %.o, $(wildcard *.c))

clean:
	-rm -f *.o
