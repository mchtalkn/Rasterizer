#include "CameraHandler.h"

// shorthand notation, filled in constructor once.
double r,l,t,b,n,f;
int nx, ny;

GeneratedMesh& CameraHandler::apply_modeling_transformation(GeneratedMesh& m)
{
    for(int i=0; i<m.original.numberOfTransformations; i++){
        TransformationFetchRun(m.original.transformationIds[i], m.original.transformationTypes[i], scene);
    }
	return m;
}


void CameraHandler::generate_orthographic_matrix()
{
    double val[4][4] = {[2/(r-l), 0, 0,  -1*(r+l)/(r-l)], [ 0, 2/(t-b), 0, -1*(t+b)/(t-b)], [0,0, -2/(f-n), -(f+n)/(f-n)], [ 0,0,0,1]};
    this->orthographic =  Matrix4(val);
}

void CameraHandler::generate_perspective_matrix()
{
    double val[4][4] = {[(2*n)/(r-l), 0, (r+l)/(r-l), 0],[ 0, 2*n/(t-b), (t+b)/(t-b), 0],[ 0,0, -(f+n)/(f-n), -2*f*n/(f-n)],[ 0,0,-1,0]};
    this->perspective =  Matrix4(val);
}

GeneratedMesh& CameraHandler::apply_viewing_transformations(GeneratedMesh& m)
{
    int i,j;
    double divisor;
    generate_orthographic_matrix();
    generate_perspective_matrix();

    double viewport[4][4] = {[nx/2, 0, 0, (nx-1)/2],[ 0, ny/2, 0, (ny-1)/2],[ 0,0, 1/2, 1/2], [0,0,0,0]};
	this->viewingTrans = multiplyMatrixWithMatrix( this->orthographic, this->perspective);

	// viewingTrans for solid
	for ( i=0; i< m.generated_triangles.size() ; i++){
	    for ( j=0 ; j<3; j++){
            // viewing transformation
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(this->viewingTrans, m.generated_triangles[i].vertices[j]);
            //perspective divide
            divisor = m.generated_triangles[i].vertices[j].t;
            if(m.generated_triangles[i].vertices[j].t != 1){
                m.generated_triangles[i].vertices[j].x /= divisor;
                m.generated_triangles[i].vertices[j].y /= divisor;
                m.generated_triangles[i].vertices[j].z /= divisor;
                m.generated_triangles[i].vertices[j].t=1;
            }
            // viewport transformation = t becomes 0, do not use it anymore.
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(Matrix4(viewport), m.generated_triangles[i].vertices[j]);
        }
	}
	// viewingTrans for lines
    for ( i=0; i< m.generated_lines.size() ; i++){
        for ( j=0 ; j<3; j++){
            // viewing transformation
            m.generated_lines[i].vertices[j] = multiplyMatrixWithVec4(this->viewingTrans, m.generated_lines[i].vertices[j]);
            //perspective divide
            divisor = m.generated_triangles[i].vertices[j].t;
            if(m.generated_lines[i].vertices[j].t != 1){
                m.generated_lines[i].vertices[j].x /= divisor;
                m.generated_lines[i].vertices[j].y /= divisor;
                m.generated_lines[i].vertices[j].z /= divisor;
                m.generated_lines[i].vertices[j].t=1;
            }
            // viewport transformation = t becomes 0, do not use it anymore.
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(Matrix4(viewport), m.generated_triangles[i].vertices[j]);
        }
    }
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
    r = this->camera.right;
    l = this->camera.left;
    t = this->camera.top;
    b = this->camera.bottom;
    f = this->camera.far;
    n = this->camera.near;
    // ------------ RESOLUTION ---------
    nx = this->camera.horRes;
    ny = this->camera.verRes;
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