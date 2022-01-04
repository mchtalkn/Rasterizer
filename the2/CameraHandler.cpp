#include "CameraHandler.h"

GeneratedMesh& CameraHandler::apply_modeling_transformation(GeneratedMesh& m)
{
    for(int i=0; i<m.original.numberOfTransformations; i++){
        TransformationFetchRun(m.original.transformationIds[i], m.original.transformationTypes[i], scene);
    }
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

bool CameraHandler::backface_culling(Triangle& t)
{
	return false;
}

GeneratedMesh& CameraHandler::apply_clipping(GeneratedMesh& m)
{
	// TODO:
	return m;
}

bool CameraHandler::apply_clipping(generated_line& l)
{
	// TODO: 
	float tE = 0;
	float tL = 1;
	bool is_visible = false;
	float dx = l.vertices[0].getElementAt(0) - l.vertices[1].getElementAt(0);
	float dy = l.vertices[0].getElementAt(1) - l.vertices[1].getElementAt(1);
	float dz = l.vertices[0].getElementAt(2) - l.vertices[1].getElementAt(2);
	double& x0 = l.vertices[0].x;
	double& y0 = l.vertices[0].y;
	double& z0 = l.vertices[0].z;
	double& x1 = l.vertices[1].x;
	double& y1 = l.vertices[1].y;
	double& z1 = l.vertices[1].z;
	if (visible(dx, xmin - x0, tE, tL)) // left
		if (visible(-dx, x0 - xmax, tE, tL)) // right
			if (visible(dy, ymin - y0, tE, tL)) // bottom
				if (visible(-dy, y0 - ymax, tE, tL)) // top
					if (visible(dz, zmin - z0, tE, tL)) // front
						if (visible(-dz, z0 - zmax, tE, tL)) // back
						{
							is_visible = true;
							if (tL < 1) {
								x1 = x0 + dx * tL;
								y1 = y0 + dy * tL;
								z1 = z0 + dz * tL;
							}if (tE > 0) {
								x0 = x0 + dx * tE;
								y0 = y0 + dy * tE;
								z0 = z0 + dx * tE;
							}
							return true;
						}
	return false; // TODO: not visible what to do set null,s add field etc ?
}

vector<generated_triangle> CameraHandler::apply_clipping(generated_triangle& m)
{
	
	return vector<generated_triangle>();
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

bool visible(float den, float num, float& te, float& tl)
{
	float t;
	if (den > 0) {
		t = num / den;
		if (t > tl) return false;
		if (t > te) te = t;
	}
	else if (den < 0) {
		t = num / den;
		if (t < te) return false;
		if (t < tl) tl = t;
	}
	else if(num >0) {
		return false;
	}
	return true;
}

Transformation& TransformationFetchRun(int id, char type, Scene s){
    switch(type){
        case 't' : {
            for(int i=0; i< s.translations.size(); i++){
                if(s.translations[i]->id==id) { Transformation(*(s.translations[i]); break; }
            }
            break;
        }
        case 's' : {
            for(int i=0; i< s.scalings.size(); i++){
                if(s.scalings[i]->id==id) { Transformation(*(s.scalings[i]); break; }
            }
            break;
        }
        case 'r' : {
            for(int i=0; i< s.rotations.size(); i++){
                if(s.rotations[i]->id==id) { Transformation(*(s.rotations[i]); break; }
            }
            break;
        }
    }
}