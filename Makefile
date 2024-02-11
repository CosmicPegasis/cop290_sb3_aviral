all: include/*.hpp src/*.cpp
	g++ -I include/ src/*.cpp -o main

debug: include/*.hpp src/*.cpp
	g++ -g -I include/ src/*.cpp -o debug
clean:
	rm main debug
