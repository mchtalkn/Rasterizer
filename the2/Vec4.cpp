
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

Vec4 Vec4::operator-( Vec4& rhs)
{
    rhs.make_t_1();
    this->make_t_1();
    Vec4 res;
    res.x = this->x - rhs.x;
    res.y = this->y - rhs.y;
    res.z = this->z - rhs.z;
    if(this->t != rhs.t) cout << "T'S ARE NOT EQUAL IN VEC4 + OVERLOAD." << endl;
    res.colorId = 0;    // temp field.
    return res;
}

Vec4 Vec4::operator+( Vec4& rhs)
{
    rhs.make_t_1();
    this->make_t_1();
    Vec4 res;
    res.x = this->x + rhs.x;
    res.y = this->y + rhs.y;
    res.z = this->z + rhs.z;
    if(this->t != rhs.t) cout << "T'S ARE NOT EQUAL IN VEC4 + OVERLOAD." << endl;
    res.colorId = 0;    // temp field.
    return res;
}

Vec4 Vec4::operator*(const float rhs)
{
    this->make_t_1();
    Vec4 res;
    res.x = this->x * rhs;
    res.y = this->y * rhs;
    res.z = this->z * rhs;
    res.colorId = 0;    // temp field.
    return res;
}

Vec4& Vec4::operator=(const Vec4& v){
    this->x=v.x; this->y=v.y; this->z=v.z; this->t=v.t; this->colorId=v.colorId;
    return *this;
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}

void Vec4::make_t_1(){
    if(this->t == 0){
        cout <<"t is 0 !!!!! ALERT" << endl;
    }
    if(this->t != 1){
        double divisor = this->t;
        this->x /= divisor;
        this->y /= divisor;
        this->z /= divisor;
        this->t =1;
    }
}

Vec3 Vec4::transferToVec3(){
    return Vec3(this->x, this->y, this->z, this->colorId);
}