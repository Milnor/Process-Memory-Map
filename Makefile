CC=gcc
CFLAGS=-Wall
OUTDIR=release
DBGDIR=debug
PY=python3

.PHONY: all
all: procmemmap debug

mmio.h: generate_mmio_header.py
	$(PY) $< 

procmemmap: memory_map.c mmio.h
	$(CC) $(CFLAGS) $? -o $(OUTDIR)/$@

debug: memory_map.c
	$(CC) $(CFLAGS) -g $? -o $(DBGDIR)/procmemmap

.PHONY: clean
clean:
	rm -rf $(OUTDIR)/*
	rm -rf $(DBGDIR)/*
	rm mmio.h

