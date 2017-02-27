CC=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall -O2

.PHONY: all

all: Recon

Recon: main.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f *.o Recon
