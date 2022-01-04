#include "GeneratedMesh.h"

GeneratedMesh::GeneratedMesh(Mesh& original_, Scene& s):original(original_)
{
	for (Triangle& t : original.triangles) generated_triangles.push_back(generated_triangle(t,s));
}

generated_triangle::generated_triangle(Triangle& t, Scene& s)
{
	vertices[0] = *s.vertices[t.vertexIds[0]];
	vertices[1] = *s.vertices[t.vertexIds[1]];
	vertices[2] = *s.vertices[t.vertexIds[2]];
}
