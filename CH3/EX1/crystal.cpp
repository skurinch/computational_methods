// A certain crystal has the shape of a parallelpiped and is situated with ony vertex at the origin. Determine the
// volume of this crystal and check the result.

#include <iostream>
#include <Eigen/Dense>

int main()
{
    Eigen::Vector3d a(3.0, 0.0, 0.0);
    Eigen::Vector3d b(0.5, 2.0, 0.0);
    Eigen::Vector3d c(0.3, 0.2, 1.5);

    // use the scalar triple product
    double volume = std::abs(a.dot(b.cross(c)));

    std::cout << "Volume using Eigen::Vector3D = " << volume << std::endl;

    // use the determinant
    Eigen::Matrix3d M;

    M << 3.0, 0.5, 0.3,
        0.0, 2.0, 0.2,
        0.0, 0.0, 1.5;

    double volume_matrix = std::abs(M.determinant());

    std::cout << "Volume using Eigen::Matrix3D = " << volume_matrix << std::endl;

    return 0;
}