CC=gcc
CFGLASS=-Wall	-g	-c	-o2

all:server client

server:server.o
	$(CC)	$<	-o	$@
client:client.o
	$(CC)	$<	-o	$@
.PHONY:
	clean
clean:
	$(RM) -rf *.o a.out server client
