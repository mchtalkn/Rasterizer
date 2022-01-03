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
	vector<GeneratedMesh> generated_meshes; //meshes after transformations and etc.
	vector<vector<Color>> image;
	GeneratedMesh generate_mesh(Mesh& m); // primitive assembly 
	Mesh& apply_modelling_transformation(Mesh& m); // applies given modelling transformation mesh
	void generate_orthographic_matrix(); //calculates and sets orthographic transformation matrix
	void generate_perspective_matrix(); //calculates and sets perspective transformation matrix
	Mesh& apply_viewing_transformations(Mesh& m); // applies viewing transformations to mesh
	Mesh& apply_culling(Mesh& m); // applies selected culling algorithm to mesh
	Mesh& apply_clipping(Mesh& m); // applies selected clipping to mesh
	CameraHandler(Camera& camera, Scene& scene);
	//TODO: transformation functions, concatanetion of transformations
};
