#pragma once
#include "Mesh.h"
#include "Vec3.h"
#include "Scene.h"
struct generated_triangle {
	Vec4 vertices[3];
	generated_triangle(Triangle& t, Scene& s);
};
class GeneratedMesh
{
	Mesh& original;
	vector<generated_triangle> generated_triangles;
	GeneratedMesh(Mesh& original, Scene& s);
};
