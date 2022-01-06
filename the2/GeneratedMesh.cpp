#include "GeneratedMesh.h"



GeneratedMesh::GeneratedMesh(Mesh& original_, Scene& s) :original(original_)
{
	if (original.type == 1)
		for (Triangle& t : original.triangles)
			generated_triangles.push_back(generated_triangle(t, s));
}

void GeneratedMesh::set_lines()
{
	for (generated_triangle& t : generated_triangles) {
		generated_lines.push_back(generated_line(t.vertices[0],t.vertices[1]));
		generated_lines.push_back(generated_line(t.vertices[1], t.vertices[2]));
		generated_lines.push_back(generated_line(t.vertices[2], t.vertices[0]));
	}
	
}


generated_triangle::generated_triangle(Triangle& t, Scene& s)
{
	vertices[0] = *s.vertices[t.vertexIds[0]];
	vertices[1] = *s.vertices[t.vertexIds[1]];
	vertices[2] = *s.vertices[t.vertexIds[2]];
}

generated_triangle::generated_triangle(Vec4& v1, Vec4& v2, Vec4& v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
}

generated_line::generated_line(const Vec4& v1, const Vec4& v2)
{
	vertices[0] = v1;
	vertices[1] = v2;
}
