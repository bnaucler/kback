CC = cc
TARGET = kback
SOURCE = kback.c
DESTDIR = /usr/bin
CFLAGS= -Wall -g -O2

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install:
	mv $(TARGET) $(DESTDIR)
