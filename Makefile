CP = cp
CFLAGS  = -O2 -Wall

all: urlman

urlman: urlman.c
	$(CC) $(CFLAGS) urlman.c -ourlman

install:
	$(CP) urlman /usr/bin/

clean:
	$(RM) urlman
	$(RM) *.bak *~
