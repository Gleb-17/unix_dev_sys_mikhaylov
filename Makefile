CFLAGS=-Wall -ggdb
TARGET=simple3
PREFIX=/usr/local

.PHONY: all clean install uninstall

all: $(TARGET)

simple3: simple3.c print.h libPrint.so
	cc $(CFLAGS) simple3.c -L. -lPrint -o $(TARGET)

libPrint.so: print.c print.h
	cc $(CFLAGS) print.c -shared -fPIC -o libPrint.so

clean: 
	-rm -f $(TARGET) libPrint.so *.o 2>/dev/null

install:
	install $(TARGET) $(PREFIX)/bin
	install libPrint.so $(PREFIX)/lib

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
	rm -f $(PREFIX)/lib/libPrint.so
