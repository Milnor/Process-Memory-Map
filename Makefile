CC=gcc
CFLAGS=-Wall

procmemmap: memory_map.c
	$(CC) $(CFLAGS) $? -o $@

