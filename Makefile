CC=gcc
CFLAGS=-lpthread
FILE = dpp.c
OUT_EXE = dpp

build: $(FILE)
	$(CC) -o $(OUT_EXE) $(FILE) $(CFLAGS)
clean:
	rm -f $(OUT_EXE) *.o