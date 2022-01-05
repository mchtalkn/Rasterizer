#ifndef __VEC4_H__
#define __VEC4_H__
#include "Vec3.h"
#include <iostream>
using namespace std;

class Vec4
{
public:
    double x, y, z, t;
    int colorId;


    Vec4();
    Vec4(double x, double y, double z, double t, int colorId);
    Vec4(const Vec4 &other);
    Vec4(const Vec3& other);
    double getElementAt(int index);
    void make_t_1();
    Vec4& operator=(const Vec4& v);
    Vec4& operator-(const Vec4& v);
    Vec3& transferToVec3();

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

#endif