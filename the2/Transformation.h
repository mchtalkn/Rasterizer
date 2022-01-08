#pragma once
#include "Matrix4.h"
#include "Scaling.h"
#include "Translation.h"
#include "Rotation.h"
#include "Mesh.h"
#include "GeneratedMesh.h"
#include "Helpers.h"
class Transformation
{
public:
	Matrix4 matrix;
	Transformation();
	Transformation(const Scaling& s);
	Transformation(const Rotation& r);
	Transformation(const Translation& t);
	Transformation& concatenate(const Transformation& t); // changes transformation in the argument and returns it
	GeneratedMesh& apply(GeneratedMesh& m); // Changes argument and returns it
	generated_triangle& apply(generated_triangle& t); // Changes argument and returns it
	Vec4& apply(Vec4& v); // Changes argument and returns it
};
