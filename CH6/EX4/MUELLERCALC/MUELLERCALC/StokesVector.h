#ifndef _STOKESVECTOR_H_
#define _STOKESVECTOR_H_

#include <iostream>

class StokesVector {

 public:

  enum Type {Horizontal, Vertical, Diagonal,
             Antidiagonal, Right, Left};

  // construct a zero vector
  inline StokesVector();

  // construct a type from a predefined type
  inline StokesVector(Type type);

  // construct from four numbers
  inline StokesVector(double x0, double x1,
                      double x2, double x3);

  // compute the degree of polarization
  inline double polarization() const;

  // access to individual elements
  inline double & operator() (unsigned int i);

  inline const double & operator() (unsigned int i) const;

 private:

  double x0, x1, x2, x3;

};

inline std::ostream & operator << (std::ostream & o,
                                   const StokesVector & v);

inline StokesVector operator * (const StokesVector & v,
                                const double & c);

inline StokesVector operator * (const double & c,
                                const StokesVector & v);

#include "StokesVector.icc"

#endif