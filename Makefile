CC=gcc
CFLAGS=-lpthread
FILE = dpp_with_deadlock.c
OUT_EXE = dpp

build: $(FILE)
	$(CC) -o $(OUT_EXE) $(FILE) $(CFLAGS)
clean:
	rm -f $(OUT_EXE) *.o
