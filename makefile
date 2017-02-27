CC=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall -O2

.PHONY: all

all: Recon

Recon: obj/main.o obj/cansend.o obj/recon.o obj/properties.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.cpp 
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/properties.o: src/properties.cpp inc/properties.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/cansend.o: src/cansend.c inc/lib.h
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/recon.o: src/recon.cpp inc/recon.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o Recon
