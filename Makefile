CXXFLAGS = -std=c++11 -Wall -Weffc++
LDLIBS = -lglut -framework OpenGL
LD = clang++
CC = clang++
CXX = clang++

all: mv

clean:
	rm -f *.o

mv: mv.o model.o
