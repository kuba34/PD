CC=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall -O2

.PHONY: all

all: Recon

Recon: obj/main.o obj/cansend.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.cpp 
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/cansend.o: src/cansend.c inc/lib.h inc/cansend.h
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o Recon
