CPPFLAGS=-I.. -g -O0
INCLUDES=../MUELLERCALC/MuellerMatrix.h ../MUELLERCALC/MuellerMatrix.icc \
         ../MUELLERCALC/StokesVector.h ../MUELLERCALC/StokesVector.icc

../local/bin/polarization:polarization.cpp $(INCLUDES)
	mkdir -p ../local/bin
	$(CXX) $< $(CPPFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -rf *~ ../MUELLERCALC/*~ ../local