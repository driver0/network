CC=gcc
CFGLASS=-Wall -g -c -O2

all:server client

server:server.o
	$(CC) $< -o $@

client:client.o
	$(CC) $< -o $@

%.o:%.c
	$(CC) $(CFGLASS) $< -o $@

.PHONY:
	clean

clean:
	$(RM) -rf *.o a.out server client
