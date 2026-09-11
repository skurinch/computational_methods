The volume is calculated using the scalar triple product, 
$$V = |\mathbf{a}\cdot(\mathbf{b}\times\mathbf{c})|,$$ 
and checked using the determinant, $$V = |\det(M)|.$$
For $\mathbf{a}=(3,0,0)$, $\mathbf{b}=(0.5,2,0)$, and $\mathbf{c}=(0.3,0.2,1.5)$, both methods give 
$V=9.0$ cubic units as shown below:

```shell
(base) shalinikv@Shalinis-MacBook-Pro EX1 % ./crystal                 
Volume using Eigen::Vector3D = 9
Volume using Eigen::Matrix3D = 9
```
