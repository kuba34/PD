CC=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall -O2

.PHONY: all

all: Recon

Recon: obj/main.o obj/PD.o obj/cansend.o obj/recon.o obj/properties.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: main.cpp 
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/properties.o: src/properties.cpp inc/properties.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/PD.o: src/PD.cpp inc/PD.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/cansend.o: src/cansend.c inc/lib.h inc/cansend.h
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/recon.o: recon.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o Recon
