CXXFLAGS = -std=c++11 -Wall -Weffc++
LD = clang++
CC = clang++
CXX = clang++

all: mv

clean:
	rm -f *.o

mv: mv.o model.o
