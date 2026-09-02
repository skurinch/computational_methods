## _observations_

I modified the modular LEGENDRE program to plot spherical Bessel functions instead of Legendre polynomials. I updated the header declarations 
and comments, renamed the plotting function from `plotLegendre()` to `plotSphBessel()`, and changed the calculation to use `std::sph_bessel(l, x)`. 
I also changed the plotting range to \(x=0\)–\(20\), since spherical Bessel functions are defined for nonnegative \(x\).To support the new plotting 
code, I added the `<algorithm>` header for `std::min_element()` and `std::max_element()`, allowing the output to automatically scale to the 
function's range.

```
(base) shalinikv@Shalinis-MacBook-Pro src % ./sphBessel 3

Spherical Bessel Function j_3(x)

                 ***                    |                                        
                *   *                   |                                        
                                        |                                        
               *     *                  |                                        
                                        |                                        
              *       *                 |                                        
                                        |                                        
             *                          |                                        
                       *                |                                        
                                        |                                        
            *                           |                                        
                                        |                                        
                        *               |                                        
           *                            |                                        
                                        |                                        
          *              *              |                                        
                                        |                                        
                                        |       *                                
         *                              |     ** **                              
                          *             |                                        
--------*-------------------------------+----*-----*-----------------------------
                                        |   *       *                   *****    
       *                                |                              *     *   
                           *            |  *         *                *       *  
      *                                 |                                        
     *                                  |             *              *         * 
   **                                   | *                         *           *
***                         *           |              *                         
                                        |*                         *             
                                        |               *         *              
                             *          |                                        
                                        *                *       *               
                                        |                 *     *                
                                        |                  *   *                 
                              *        *|                   ***                  
                                        |                                        
                               *      * |                                        
                                        |                                        
                                *    *  |                                        
                                 *  *   |                                        
                                  **    | 
```
