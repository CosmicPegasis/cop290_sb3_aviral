all: include/*.hpp src/*.cpp
	g++ -I include/ src/*.cpp -o main -fopenmp

debug: include/*.hpp src/*.cpp
	g++ -g -I include/ src/*.cpp -o debug -fopenmp
clean:
	rm main debug
