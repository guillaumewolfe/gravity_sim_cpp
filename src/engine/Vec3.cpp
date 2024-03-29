#include <cmath>
#include <tuple>
#include "engine/Vec3.h"

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

double Vec3::norm() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const
{
    double magnitude = norm();
    if (magnitude == 0)
    { // To prevent division by zero
        return *this;
    }
    return Vec3(x / magnitude, y / magnitude, z / magnitude);
}

std::tuple<double, double, double> Vec3::toPolar() const
{
    double r = norm();
    double theta = std::atan2(y, x);
    double phi = std::acos(z / r);
    return {r, theta, phi};
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator+(const Vec3 &other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vec3 Vec3::operator*(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

double Vec3::dot(const Vec3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const
{
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

std::string Vec3::print()
{
    return "Vec: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

glm::vec3 Vec3::toGlm() {
    return glm::vec3(x, y, z);
}

glm::vec3 Vec3::toGlm() const {
    return glm::vec3(x, y, z);
}