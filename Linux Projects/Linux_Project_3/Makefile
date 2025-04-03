# Makefile for your project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -std=gnu99 -pthread

# Targets and dependencies
all: dbclient dbserver

dbclient: dbclient.c
	$(CC) $(CFLAGS) dbclient.c -o dbclient

dbserver: dbserver.c
	$(CC) $(CFLAGS) dbserver.c -o dbserver

clean:
	rm -f dbclient dbserver