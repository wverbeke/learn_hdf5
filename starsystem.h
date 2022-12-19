#ifndef STARSYSTEM_H
#define STARSYSTEM_H

#include "vec.h"
#include "body.h"
#include <vector>

class StarSystem{

    public:
        StarSystem(const std::vector<Body<double>>& bodies) : _bodies(bodies) {}

        const Body<double> & operator[](const unsigned index) const{ return _bodies[index]; }
        const size_t size() const{ return _bodies.size(); }

    private:
        std::vector<Body<double>> _bodies;
};
#endif
