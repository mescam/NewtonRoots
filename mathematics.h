#ifndef MATH_H
#define MATH_H

namespace Math {
long double newtonRoots(unsigned int n, long double *a, long double &x,
                        unsigned int mit, long double eps,
                        long double &w, unsigned int &it, char &st);


long double horner(unsigned int n, long double *a, long double x, long double &df);
}
#endif // MATH_H
