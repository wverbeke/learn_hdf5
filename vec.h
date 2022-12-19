#ifndef VEC_H
#define VEC_H
template <typename T> class Vec{

    public:
        using value_type = T;
        Vec(): _x(0.0), _y(0.0), _z(0.0) {}
        Vec(T x, T y, T z): _x(x), _y(y), _z(z) {}

        T x() const{ return _x; }
        T y() const{ return _y; }
        T z() const{ return _z; }

    private:
        T _x;
        T _y;
        T _z;
};
#endif
