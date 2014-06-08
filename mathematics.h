#ifndef MATH_H
#define MATH_H
#include "Interval/IntervalArithmetic.h"

namespace Math {
long double newtonRoots(unsigned int n, long double *a, long double &x,
                        unsigned int mit, long double eps,
                        long double &w, unsigned int &it, char &st);


long double horner(unsigned int n, long double *a, long double x, long double &df);

intervalarth::interval intervalNewtonRoots(unsigned int n, intervalarth::interval *a,
                                           intervalarth::interval &x,
                                           unsigned int mit, long double eps,
                                           intervalarth::interval &w, unsigned int &it, char &st);

intervalarth::interval intervalHorner(unsigned int n, intervalarth::interval *a,
                                      intervalarth::interval x, intervalarth::interval &df);

intervalarth::interval readInterval(std::string text);

intervalarth::interval intervalAbs(intervalarth::interval interval);

intervalarth::interval intervalMax(intervalarth::interval a, intervalarth::interval b);
}
#endif // MATH_H
