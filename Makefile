CC	= gcc
CFLAGS	= -O2 -Wall -Wno-unused-result
LIBS	= -lX11
PROGRAM	= crop
SRCS	= main.c
DESTDIR	= ~/bin

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(PROGRAM) $(LIBS)

install:
	cp $(PROGRAM) $(DESTDIR)
	cp imgur_upload.py $(DESTDIR)

uninstall:
	rm $(DESTDIR)/$(PROGRAM)
	rm $(DESTDIR)/imgur_upload.py

.PHONY: all install uninstall clean
clean:
	rm -f *.o $(PROGRAM)
