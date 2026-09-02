OFILES=parseInput.o getCanvas.o plotSphBessel.o

CXX = g++-16
CXXFLAGS = -I../include -std=c++20

sphBessel: sphBessel.o libSphBessel.a
	$(CXX) -o $@ $< -L. -lSphBessel

libSphBessel.a: $(OFILES)
	ar rc $@ $?

clean:
	rm -f *.a *.o *~ sphBessel