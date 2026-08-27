CXX = g++
CXXFLAGS = -Wall -std=c++11 -fPIC

LIB = libPlus.so
TARGET = plus

all: $(TARGET)

$(LIB): parse.o add.o print.o
	$(CXX) -shared -o $(LIB) parse.o add.o print.o

plus: main.o $(LIB)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o -L. -lPlus

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

parse.o: parse.cpp
	$(CXX) $(CXXFLAGS) -c parse.cpp

add.o: add.cpp
	$(CXX) $(CXXFLAGS) -c add.cpp

print.o: print.cpp
	$(CXX) $(CXXFLAGS) -c print.cpp

clean:
	rm -f *.o $(LIB) $(TARGET)