#include "mathematics.h"
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "Interval/IntervalArithmetic.h"
#include <cmath>
namespace Math {

intervalarth::interval intervalAbs(intervalarth::interval interval)
{
    intervalarth::interval res;
    res.a = (interval.a <= 0 && interval.b >= 0) ? 0 : std::min(std::abs(interval.a), std::abs(interval.b));
    res.b = std::max(std::abs(interval.a), std::abs(interval.b));
    return res;
}

intervalarth::interval intervalMax(intervalarth::interval a, intervalarth::interval b) {
    intervalarth::interval res;
    res.a = std::max(a.a, b.a);
    res.b = std::max(a.b, b.b);
    return res;
}

intervalarth::interval intervalNewtonRoots(unsigned int n, intervalarth::interval *a, intervalarth::interval &x,
                                           unsigned int mit, long double eps, intervalarth::interval &w,
                                           unsigned int &it, char &st)
{
    intervalarth::IntervalArithmetic ia;

    it = 1;
    st = 3;

    while(it <= mit && st == 3) {
        qDebug() << "Iteration" << it;
        intervalarth::interval df, x2;
        w = Math::intervalHorner(n, a, x, df);
        if(df.a <= 0 && df.b >= 0) {
            qDebug() << "df contains 0, breaking";
            st = 2;
            break;
        }
        x2 = ia.ISub(x, ia.IDiv(w, df));
        intervalarth::interval test;
        test = intervalMax(intervalAbs(x), intervalAbs(x2));
        if(test.a <= 0 && test.b >= 0) {
            qDebug() << "division by zero in eps test";
            st = 0;
            x = test;
            break;
        }
        intervalarth::interval checkeps = ia.IDiv(
                    intervalAbs(ia.ISub(x2, x)),
                    intervalMax(intervalAbs(x), intervalAbs(x2))
                    );
        if(checkeps.a < eps && checkeps.b < eps) {
            qDebug() << "epsilon breaking point";
            st = 0;
        }
        x = x2;
        ++it;
    }
    it--;
    return x;
}

long double newtonRoots(unsigned int n, long double *a, long double &x,
                        unsigned int mit, long double eps, long double &w,
                        unsigned int &it, char &st)
{
    //qDebug() << QString::fromStdString(std::to_string(eps));
    it = 1;
    st = 3;
    while(it <= mit && st == 3)
    {
        qDebug() << "Iteration" << it;
        long double df;
        w = Math::horner(n, a, x, df);
        long double x2 = x - (w/df);
        if(df == 0) {
            st = 2;
            break;
        }
        if(std::max(std::abs(x), std::abs(x2)) == 0) {
            x = 0;
            st = 0;
            break;
        }
        if((std::abs(x2 - x) / std::max(std::abs(x2), std::abs(x))) < eps)
            st = 0;

        x = x2;
        ++it;
    }
    it--;
    return x;
}

long double horner(unsigned int n, long double *a, long double x, long double &df)
{
    long double result = a[n];
    df = result;
    for(int i = n - 1; i >= 0; i--) {
        result = result * x + a[i];
        if(i > 0)
            df = df * x + result;
    }
    qDebug() << "f ="
             << QString::fromStdString(boost::lexical_cast<std::string>(result))
             << ", df = "
             << QString::fromStdString(boost::lexical_cast<std::string>(df));
    return result;
}

intervalarth::interval intervalHorner(unsigned int n, intervalarth::interval *a,
                                      intervalarth::interval x, intervalarth::interval &df)
{
    intervalarth::IntervalArithmetic ia;
    intervalarth::interval result = a[n];
    df = result;
    for(int i = n - 1; i >= 0; i--) {
        result = ia.IAdd(ia.IMul(result, x), a[i]);
        if(i > 0)
            df = ia.IAdd(ia.IMul(df, x), result);
    }
    return result;
}

intervalarth::interval readInterval(std::string text) {
    intervalarth::interval interval;
    intervalarth::IntervalArithmetic ia; //i don't even know
    try {
        boost::lexical_cast<long double>(text);
        interval = ia.IntRead(text);
    } catch(boost::bad_lexical_cast) {
        std::vector<std::string> tokens;
        boost::split(tokens, text, boost::is_any_of(" "));
        interval.a = boost::lexical_cast<long double>(tokens[0]);
        interval.b = boost::lexical_cast<long double>(tokens[1]);
    }
    return interval;
}

} //end of Math namespace
