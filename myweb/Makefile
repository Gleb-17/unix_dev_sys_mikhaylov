CFLAGS=-Wall -ggdb
TARGET=myweb

.PHONY: all clean

all: $(TARGET)

myweb: myweb.c
	gcc $(CFLAGS) myweb.c -o $(TARGET)

clean:
	rm -f myweb
 