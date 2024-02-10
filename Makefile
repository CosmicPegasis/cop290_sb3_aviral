all: include/*.hpp src/*.cpp
	g++ -I include/ src/* -o main

debug: include/*.hpp src/*.cpp
	g++ -g -I include/ src/* -o debug
clean:
	rm main debug
