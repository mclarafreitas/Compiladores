CXX = g++
CXXFLAGS = -Wall -g

all: seu_programa

seu_programa: tradutor.cpp ast.cpp
	$(CXX) $(CXXFLAGS) -o seu_programa tradutor.cpp ast.cpp

clean:
	rm -f seu_programa
