#ifndef _MUELLERMATRIX_H_
#define _MUELLERMATRIX_H_

#include "StokesVector.h"

class MuellerMatrix {

 public:

  enum Type { Horizontal, Vertical, Diagonal,
              Antidiagonal, Right, Left,
              Identity };

  // construct the identity matrix
  inline MuellerMatrix();

  // construct a type from a predefined type
  inline MuellerMatrix(Type type);

  // construct from sixteen numbers
  inline MuellerMatrix(double a00, double a01, double a02, double a03,
                       double a10, double a11, double a12, double a13,
                       double a20, double a21, double a22, double a23,
                       double a30, double a31, double a32, double a33);

  // access to individual elements
  inline double & operator() (unsigned int i, unsigned int j);

  inline const double & operator() (unsigned int i, unsigned int j) const;

 private:

  double a00, a01, a02, a03;
  double a10, a11, a12, a13;
  double a20, a21, a22, a23;
  double a30, a31, a32, a33;

};

inline std::ostream & operator << (std::ostream & o,
                                   const MuellerMatrix & m);

inline MuellerMatrix operator * (const MuellerMatrix & m1,
                                 const MuellerMatrix & m2);

inline StokesVector operator * (const MuellerMatrix & m,
                                const StokesVector & v);

#include "MuellerMatrix.icc"

#endif