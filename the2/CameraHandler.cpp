#include "CameraHandler.h"

GeneratedMesh& CameraHandler::apply_modelling_transformation(GeneratedMesh& m)
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

void CameraHandler::render(GeneratedMesh& m)
{
}

CameraHandler::CameraHandler(Camera& camera_, Scene& scene_):camera(camera_),scene(scene_)
{
}
