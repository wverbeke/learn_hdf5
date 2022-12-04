#ifndef BODY_H
#define BODY_H

#include "vec.h"

class Body{

    public:
        Body(const Vec& x, const Vec& v, const double m): _x(x), _v(v), _m(m) {}

        const Vec& x() const{ return _x; }
        const Vec& v() const{ return _v; }
        const double m() const{ return _m; }

    private:
        Vec _x;
        Vec _v;
        double _m;
};
#endif
