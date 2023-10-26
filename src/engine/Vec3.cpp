#include <cmath>
#include <tuple>
#include "engine/Vec3.h"



Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z){}

double Vec3::norm() const{
    return std::sqrt(x*x+y*y+z*z);
}

std::tuple<double,double,double> Vec3::toPolar() const{
    double r = norm();
    double theta = std::atan2(y,x);
    double phi = std::acos(z/r);
    return {r, theta, phi};
}
