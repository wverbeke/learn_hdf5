#ifndef BODY_H
#define BODY_H

#include "vec.h"

template<typename T> class Body{

    public:
        Body(const Vec<T>& x, const Vec<T>& v, const T m): _x(x), _v(v), _m(m) {}

        const Vec<T>& x() const{ return _x; }
        const Vec<T>& v() const{ return _v; }
        const T m() const{ return _m; }

    private:
        Vec<T> _x;
        Vec<T> _v;
        T _m;
};
#endif
