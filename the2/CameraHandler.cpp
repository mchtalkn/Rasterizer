#include "CameraHandler.h"

GeneratedMesh& CameraHandler::apply_modeling_transformation(GeneratedMesh& m)
{
	// TODO:
	return m;
}

void CameraHandler::generate_orthographic_matrix()
{
	// TODO:
}

void CameraHandler::generate_perspective_matrix()
{
	// TODO:
}

GeneratedMesh& CameraHandler::apply_viewing_transformations(GeneratedMesh& m)
{
	// TODO:
	return m;
}

GeneratedMesh& CameraHandler::apply_culling(GeneratedMesh& m)
{
	// TODO:
	return m;
}

GeneratedMesh& CameraHandler::apply_clipping(GeneratedMesh& m)
{
	// TODO:
	return m;
}

void CameraHandler::render()
{
	for (GeneratedMesh& m : generated_meshes) render(m);
	//TODO write image to file
}

void CameraHandler::render(GeneratedMesh& m)
{
	if (m.original.type == 0) {
		for (generated_line& l : m.generated_lines) render(l);
	}
	else if (m.original.type == 1) {
		for (generated_triangle& t : m.generated_triangles) render(m);
	}
}

void CameraHandler::render(generated_triangle& t)
{
}

void CameraHandler::render(generated_line& l)
{
}

CameraHandler::CameraHandler(Camera& camera_, Scene& scene_):camera(camera_),scene(scene_)
{
}
