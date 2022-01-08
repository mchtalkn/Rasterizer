#pragma once

#include "Helpers.h" // includes vec4 and vec3.
#include "Camera.h"
#include "Scene.h"
#include "GeneratedMesh.h"
#include <vector>
#include "Transformation.h"
using namespace std;

//void modelingTransformationFetchRun(int id, char type, Scene &s);

class CameraHandler
{
public:
	Camera& camera;
	Scene& scene;
	Matrix4 cameraTrans;
	Matrix4 orthographic;
	Matrix4 perspective;
    Matrix4 viewingTrans;
	vector<vector<Color>>& image;
	vector<GeneratedMesh> generated_meshes; //meshes after transformations and etc.
	//float xmin, xmax, ymin, ymax, zmin, zmax; // should be set after generating perspective/ortoghraphic
	GeneratedMesh& apply_modeling_transformation(GeneratedMesh& m); // applies given modelling transformation mesh
	void generate_orthographic_matrix(); //calculates and sets orthographic transformation matrix
	void generate_perspective_matrix(); //calculates and sets perspective transformation matrix
	void generate_cameraTrans_matrix(); //calculates and sets orthographic transformation matrix
	GeneratedMesh& apply_viewing_transformations(GeneratedMesh& m); // applies viewing transformations to mesh, do not divide last term
	GeneratedMesh& apply_culling(GeneratedMesh& m); // applies selected culling algorithm to mesh
	bool backface_culling(generated_triangle& t); // returns whether triangle should be culled or not
	GeneratedMesh& apply_clipping(GeneratedMesh& m); // applies selected clipping to mesh
	bool apply_clipping(generated_line& l);
	vector<generated_triangle> apply_clipping(generated_triangle& m);
	void render();
	void render(GeneratedMesh& m);
	void render(generated_triangle& t);
	void render(generated_line& l);
	CameraHandler(Camera& camera, Scene& scene);
	//TODO: transformation functions, concatanetion of transformations
};
bool visible(float den, float num, float& te, float& tl);



