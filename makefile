CC=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs` -std=c++11 -Wall

.PHONY: all

all: Recon

Recon: obj/main.o obj/PD.o obj/can.o obj/recon.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: main.cpp 
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/PD.o: src/PD.cpp inc/PD.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/can.o: src/can.cpp inc/can.hpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

obj/recon.o: recon.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o Recon
