CC=gcc
CXX=g++
CXXFLAGS=`pkg-config opencv --cflags` -Iinc -Wall
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall

.PHONY: all

all: Recon

Recon: obj/cansend.o obj/lib.o obj/main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.cpp obj/cansend.o
	$(CXX) -c $< -o $@ $(CXXFLAGS)

obj/cansend.o: src/cansend.c inc/lib.h inc/cansend.h
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/lib.o: src/lib.c inc/lib.h
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o Recon
