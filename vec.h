#ifndef VEC_H
#define VEC_H
class Vec{

    public:
        Vec(): _x(0.0), _y(0.0), _z(0.0) {}
        Vec(double x, double y, double z): _x(x), _y(y), _z(z) {}

        double x() const{ return _x; }
        double y() const{ return _y; }
        double z() const{ return _z; }

        //std::vector<double> to_vector() const{ return std::vector<double>{_x, _y, _z}; }

    private:
        double _x;
        double _y;
        double _z;
};
#endif
