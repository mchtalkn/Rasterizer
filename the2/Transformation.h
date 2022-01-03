#pragma once
#include "Matrix4.h"
#include "Scaling.h"
#include "Translation.h"
#include "Rotation.h"
#include "Mesh.h"
#include "Vec4.h"
class Transformation
{
	Matrix4 matrix;
	Transformation(const Scaling& s);
	Transformation(const Rotation& r);
	Transformation(const Translation& t);
	Transformation& concatenate(Transformation& t); // changes transformation in the argument and returns it
	Mesh& apply(Mesh& m); // Changes argument and returns it
	Triangle& apply(Triangle& t); // Changes argument and returns it
	Vec4& apply(Vec4& v); // Changes argument and returns it
};
