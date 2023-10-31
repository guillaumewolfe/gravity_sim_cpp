#include <cmath>
#include <tuple>
#include "engine/Vec3.h"



Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z){}

double Vec3::norm() const{
    return std::sqrt(x*x+y*y+z*z);
}

Vec3 Vec3::normalize() const {
    double magnitude = norm();
    if (magnitude == 0) { // To prevent division by zero
        return *this;
    }
    return Vec3(x / magnitude, y / magnitude, z / magnitude);
}

std::tuple<double,double,double> Vec3::toPolar() const{
    double r = norm();
    double theta = std::atan2(y,x);
    double phi = std::acos(z/r);
    return {r, theta, phi};
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}