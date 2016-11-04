CC=gcc
CFLAGS=-lpthread
FILE = dpp.c
FILE2 = dpp_with_deadlock.c
OUT_EXE = dpp
OUT_EXE2 = dppwd

build: $(FILE)
	$(CC) -o $(OUT_EXE) $(FILE) $(CFLAGS)
	$(CC) -o $(OUT_EXE2) $(FILE2) $(CFLAGS)
clean:
	rm -f $(OUT_EXE) *.o
