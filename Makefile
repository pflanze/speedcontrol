
all:
	gcc -o dvdspeedcontrol -Wall dvdspeedcontrol.c

install: all
	install dvdspeedcontrol /usr/local/bin/
