
#include "Vec4.h"
#include <iomanip>

using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->colorId = -1;
}

Vec4::Vec4(double x, double y, double z, double t, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = colorId;
}
Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;
}

Vec4::Vec4(const Vec3& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = 1;
    this->colorId = other.colorId;
}

double Vec4::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}

Vec4 Vec4::operator-(const Vec4& rhs)
{
    Vec4 v;
    v.x = x - rhs.x;
    v.y = y - rhs.y;
    v.z = z - rhs.z;
    v.t = t;
    //v.t = t - rhs.t; // I wont use this probably
    return v;
}

Vec4 Vec4::operator+(const Vec4& rhs)
{
    Vec4 v;
    v.x = x + rhs.x;
    v.y = y + rhs.y;
    v.z = z + rhs.z;
    v.t = t;
    //v.t = t - rhs.t; // I wont use this probably
    return v;
}

Vec4 Vec4::operator*(const float rhs)
{
    Vec4 v;
    v.x = x * rhs;
    v.y = y * rhs;
    v.z = z * rhs;
    v.t = t;
    return v;
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}