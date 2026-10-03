I adapted the original Jones calculus code to use Mueller calculus, which describes polarization using 
Stokes vectors and Mueller matrices instead of Jones vectors and Jones matrices.

`JonesVector` was replaced by `StokesVector`. A Stokes vector has four real components, $(I,Q,U,V),$
where `I` is the total intensity and `Q`, `U`, and `V` describe the polarization state. I kept the same predefined polarization types as in the original code: `Horizontal`, `Vertical`, `Diagonal`, `Antidiagonal`, `Right`, and `Left`.

`JonesMatrix` was replaced by `MuellerMatrix`. The main difference is that a Mueller matrix is a 4 × 4 real matrix, rather than a 2 × 2 complex matrix. I kept the same basic structure as the original code, including constructors for predefined polarization elements, element access, printing, matrix multiplication, and matrix-vector multiplication.

The implementation is split into `.h` and `.icc` files in the same way as the original Jones code. The `.h` files contain the class declarations, while the `.icc` files contain the inline implementations.

Finally, `polarization.cpp` was changed to construct Stokes vectors and Mueller matrices and apply a sequence of polarizers. The final Stokes vector gives the resulting intensity and polarization state.
