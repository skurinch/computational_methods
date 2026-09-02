OFILES=parseInput.o getCanvas.o plotLegendre.o

CXX = g++-16
CXXFLAGS = -I../include -std=c++20

legendre: legendre.o libLegendre.a
	$(CXX) -o $@ $< -L. -lLegendre

libLegendre.a: $(OFILES)
	ar rc $@ $?

clean:
	rm -f *.a *.o *~ legendre