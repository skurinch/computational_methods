CXX = g++
CXXFLAGS = -Wall -std=c++11

LIB = libPlus.a
TARGET = plus

all: $(TARGET)

$(LIB): parse.o add.o print.o
	ar rcs $(LIB) parse.o add.o print.o

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