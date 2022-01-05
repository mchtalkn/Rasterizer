#pragma once
#include "Mesh.h"
#include "Vec3.h"
#include "Scene.h"
struct generated_line {
	Vec4 vertices[2];
	generated_line(const Vec4& v1, const Vec4& v2);
};
struct generated_triangle {
	Vec4 vertices[3];
    Vec4 normal;
	generated_triangle(Triangle& t, Scene& s);
};
class GeneratedMesh
{
public:
	Mesh& original;
	vector<generated_triangle> generated_triangles;
	vector<generated_line> generated_lines;
	GeneratedMesh(Mesh& original, Scene& s);
};
