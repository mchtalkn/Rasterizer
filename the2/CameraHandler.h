#pragma once
#include "Camera.h"
#include "Scene.h"
#include "Matrix4.h"
#include "GeneratedMesh.h"
class CameraHandler
{
	Camera& camera;
	Scene& scene;
	Matrix4 orthographic;
	Matrix4 perspective;
	vector<vector<Color>> image;
	vector<GeneratedMesh> generated_meshes; //meshes after transformations and etc.
	GeneratedMesh& apply_modelling_transformation(GeneratedMesh& m); // applies given modelling transformation mesh
	void generate_orthographic_matrix(); //calculates and sets orthographic transformation matrix
	void generate_perspective_matrix(); //calculates and sets perspective transformation matrix
	GeneratedMesh& apply_viewing_transformations(GeneratedMesh& m); // applies viewing transformations to mesh, do not divide last term
	GeneratedMesh& apply_culling(GeneratedMesh& m); // applies selected culling algorithm to mesh
	GeneratedMesh& apply_clipping(GeneratedMesh& m); // applies selected clipping to mesh
	void render(GeneratedMesh& m);
	CameraHandler(Camera& camera, Scene& scene);
	//TODO: transformation functions, concatanetion of transformations
};
