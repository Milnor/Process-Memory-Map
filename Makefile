CC=gcc
CFLAGS=-Wall
OUTDIR=release
DBGDIR=debug

.PHONY: all
all: procmemmap debug

procmemmap: memory_map.c
	$(CC) $(CFLAGS) $? -o $(OUTDIR)/$@

debug: memory_map.c
	$(CC) -g $? -o $(DBGDIR)/procmemmap

.PHONY: clean
clean:
	rm -rf $(OUTDIR)/*
	rm -rf $(DBGDIR)/*

