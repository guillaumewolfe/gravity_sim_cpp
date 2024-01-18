#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <tuple>
#include <glm/glm.hpp>
#include <iostream>

class Vec3{
public:
    double x, y, z;

    Vec3(double x =0, double y = 0, double z = 0);

    double norm() const;
    Vec3 normalize() const;

    std::tuple<double,double,double> toPolar() const;

    Vec3 operator-(const Vec3& other) const;
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator*=(float scalar);
    Vec3 operator*(float scalar);
    glm::vec3 toGlm();

    

    double dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other) const;

    std::string print();

};
#endif