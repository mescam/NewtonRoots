#include "mathematics.h"
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <boost/lexical_cast.hpp>

namespace Math {
long double newtonRoots(unsigned int n, long double *a, long double &x,
                        unsigned int mit, long double eps, long double &w,
                        unsigned int &it, char &st)
{
    qDebug() << QString::fromStdString(boost::lexical_cast<std::string>(eps));
    it = 0;
    st = 3;
    while(it++ < mit && st == 3)
    {
        qDebug() << "Iteration" << it;
        long double df;
        long double w = Math::horner(n, a, x, df);
        long double x2 = x - (w/df);
        if(x == 0) {
            st = 0;
            break;
        }else if(x2 == 0) {
            x = x2;
            st = 0;
            break;
        }
        if((std::abs(x2 - x) / std::max(std::abs(x2), std::abs(x))) < eps)
            st = 0;

        x = x2;
    }
    return x;
}

long double horner(unsigned int n, long double *a, long double x, long double &df)
{
    qDebug() << "calling Horner";
    long double result = a[n];
    df = result;
    qDebug() << "before loop";
    for(int i = n - 1; i >= 0; i--) {
        result = result * x + a[i];
        if(i > 0)
            df = df * x + result;
    }
    qDebug() << "f =" << (double)result << ", df = " << (double)df;
    return result;
}

}
