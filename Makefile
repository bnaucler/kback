CC = cc
TARGET = kback
SOURCE = kback.c
DESTDIR = /usr/bin
CFLAGS= -Wall -g

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install:
	mv $(TARGET) $(DESTDIR)
