#include "CameraHandler.h"

// shorthand notation, filled in constructor once.
double r,l,t,b,n,f,
nx, ny; // these are originally int

void modelingTransformationFetchRun(int id, char type, Scene s, GeneratedMesh mesh){
    switch(type){
        case 't' : {
            for(int i=0; i< s.translations.size(); i++){
                if(s.translations[i]->translationId==id) { Transformation t(*(s.translations[i])); t.apply(mesh); break; }
            }
            break;
        }
        case 's' : {
            for(int i=0; i< s.scalings.size(); i++){
                if(s.scalings[i]->scalingId==id) { Transformation t(*(s.scalings[i])); t.apply(mesh); break; }
            }
            break;
        }
        case 'r' : {
            for(int i=0; i< s.rotations.size(); i++){
                if(s.rotations[i]->rotationId==id) { Transformation t(*(s.rotations[i])); t.apply(mesh); break; }
            }
            break;
        }
    }
}

GeneratedMesh& CameraHandler::apply_modeling_transformation(GeneratedMesh& m)
{
    for(int i=0; i<m.original.numberOfTransformations; i++){
        modelingTransformationFetchRun(m.original.transformationIds[i], m.original.transformationTypes[i], scene, m);
    }
	return m;
}


void CameraHandler::generate_orthographic_matrix()
{
    double val[4][4] = {{2/(r-l), 0, 0,  -1*(r+l)/(r-l)}, { 0, 2/(t-b), 0, -1*(t+b)/(t-b)},
                        {0,0, -2/(f-n), -(f+n)/(f-n)}, { 0,0,0,1}};
    this->orthographic =  Matrix4(val);
}

void CameraHandler::generate_perspective_matrix()
{
    double val[4][4] = {{(2*n)/(r-l), 0, (r+l)/(r-l), 0},{ 0, 2*n/(t-b), (t+b)/(t-b), 0},
                        { 0,0, -(f+n)/(f-n), -2*f*n/(f-n)},{ 0,0,-1,0}};
    this->perspective =  Matrix4(val);
}

void CameraHandler::generate_cameraTrans_matrix(){
    Vec3 &e = this->camera.pos;
    Vec3 &u = this->camera.u;
    Vec3 &v = this->camera.v;
    Vec3 &w = this->camera.w;
    double val[4][4] = {{u.x, u.y, u.z ,  -(u.x*e.x+u.y*e.y+u.z*e.z)},{v.x ,  v.y , v.z , -(v.x*e.x+v.y*e.y+v.z*e.z)},{w.x ,  w.y ,  w.z , -(w.x*e.x+w.y*e.y+w.z*e.z)},{0, 0 , 0 , 1}};
    this-> cameraTrans = Matrix4(val);
}

GeneratedMesh& CameraHandler::apply_viewing_transformations(GeneratedMesh& m)
{
    int i,j;
    double viewport[4][4] = {{nx/2, 0, 0, (nx-1)/2},{ 0, ny/2, 0, (ny-1)/2},
                             { 0,0, 1/2, 1/2}, {0,0,0,0}};

    //generate_cameraTrans_matrix();
    //generate_orthographic_matrix();
    //generate_perspective_matrix();

	// viewingTrans for solid or wireframe ==> viewport * orthographic * perspective
	for ( i=0; i< m.generated_triangles.size() ; i++){
	    for ( j=0 ; j<3; j++){
            // camera transformation
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(this->cameraTrans, m.generated_triangles[i].vertices[j]);
            // viewing transformation
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(this->viewingTrans, m.generated_triangles[i].vertices[j]);

        }
	}
	
	if (m.original.type == 1) {
		for (int j = 0; j < 3; j++) {
			// clipping before perspective divide and viewport transformation.
			//this->apply_clipping(m);
			//perspective divide
			m.generated_triangles[i].vertices[j].make_t_1();
			// viewport transformation = t becomes 0, do not use it anymore.
			m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(Matrix4(viewport), m.generated_triangles[i].vertices[j]);
		}
	}
	else {
		m.set_lines();
		// clipping before perspective divide and viewport transformation.
		apply_clipping(m);
		for (int i = 0; i < m.generated_lines.size(); i++) {
			for (int j = 0; j < 2; j++) {
				//perspective divide
				m.generated_lines[i].vertices[j].make_t_1();
				// viewport transformation = t becomes 0, do not use it anymore.
				m.generated_lines[i].vertices[j] = multiplyMatrixWithVec4(Matrix4(viewport), m.generated_lines[i].vertices[j]);
			}
		}
			
	}
	// computing normal also makes t's all 1 beforehand.
	//m.generated_triangles[i].normal = computeNormals(m.generated_triangles[i].vertices);
	return m;
}

GeneratedMesh& CameraHandler::apply_culling(GeneratedMesh& m)
{
	int i=0;
	vector<generated_triangle> new_t;
	while (i < m.generated_triangles.size()) {
		if (!backface_culling(m.generated_triangles[i])) {
			new_t.push_back(m.generated_triangles[i]);
		}
	}
	m.generated_triangles = move(new_t);
	return m;
}

bool CameraHandler::backface_culling(generated_triangle& t)
{
	
	Vec4 a,b,c;
	Vec3 n;
	a = t.vertices[2] - t.vertices[1];
	b = t.vertices[3] - t.vertices[1];
	n.x = a.y * b.z - a.z * b.y;
	n.y = a.z * b.x - a.x * b.z;
	n.z = a.x * b.y - a.y * b.x;
	return false;
}

GeneratedMesh& CameraHandler::apply_clipping(GeneratedMesh& m)
{
	// TODO:
	if (m.original.type == 1) {
		vector<generated_triangle> new_t;
		for (int i = 0; i < m.generated_triangles.size(); i++) {
			vector<generated_triangle> append = apply_clipping(m.generated_triangles[i]);
			new_t.insert(new_t.end(), append.begin(), append.end());
		}
	}
	if (m.original.type == 0) {
		for (int i = 0; i < m.generated_lines.size(); i++) {
			if (!apply_clipping(m.generated_lines[i])) {
				m.generated_lines.erase(m.generated_lines.begin() + i);
				i--;
			}
		}
	}
	return m;
}

bool CameraHandler::apply_clipping(generated_line& l)
{
	l.vertices[0].make_t_1(); // TODO these should not be there but added for testing
	l.vertices[1].make_t_1();
	float tmax = l.vertices[0].t;
	float tmin = -1 * l.vertices[0].t;
	float tE = 0;
	float tL = 1;
	bool is_visible = false;
	float dx = l.vertices[0].getElementAt(0) - l.vertices[1].getElementAt(0);
	float dy = l.vertices[0].getElementAt(1) - l.vertices[1].getElementAt(1);
	float dz = l.vertices[0].getElementAt(2) - l.vertices[1].getElementAt(2);
	double x0 = l.vertices[0].x;
	double y0 = l.vertices[0].y;
	double z0 = l.vertices[0].z;
	double x1 = l.vertices[1].x;
	double y1 = l.vertices[1].y;
	double z1 = l.vertices[1].z;

	if (visible(dx, tmin - x0, tE, tL)) // left
		if (visible(-dx, x0 - tmax, tE, tL)) // right
			if (visible(dy, tmin - y0, tE, tL)) // bottom
				if (visible(-dy, y0 - tmax, tE, tL)) // top
					if (visible(dz, tmin - z0, tE, tL)) // front
						if (visible(-dz, z0 - tmax, tE, tL)) // back
						{
							is_visible = true;
                            double colorMultiplier=0.0;
                            double V1_prev_r = scene.colorsOfVertices[l.vertices[1].colorId-1]->r;
                            double V1_prev_g = scene.colorsOfVertices[l.vertices[1].colorId-1]->g;
                            double V1_prev_b = scene.colorsOfVertices[l.vertices[1].colorId-1]->b;
                            double V0_prev_r = scene.colorsOfVertices[l.vertices[0].colorId-1]->r;
                            double V0_prev_g = scene.colorsOfVertices[l.vertices[0].colorId-1]->g;
                            double V0_prev_b = scene.colorsOfVertices[l.vertices[0].colorId-1]->b;
							if (tL < 1) {
                                l.vertices[1].x = x0 + dx * tL;
                                l.vertices[1].y = y0 + dy * tL;
                                l.vertices[1].z = z0 + dz * tL;
                                if(x1 - x0!=0)  colorMultiplier= (l.vertices[1].x-x1)/(x1 - x0);
                                else if(y1 - y0!=0)  colorMultiplier= (l.vertices[1].y-y1)/(y1 - y0);
                                else if (z1 - z0!=0)  colorMultiplier= (l.vertices[1].z-z1)/(z1 - z0);
                                scene.colorsOfVertices[l.vertices[1].colorId-1]->r = V1_prev_r + (V1_prev_r-V0_prev_r) * colorMultiplier;
                                scene.colorsOfVertices[l.vertices[1].colorId-1]->g = V1_prev_g + (V1_prev_g-V0_prev_g) * colorMultiplier;
                                scene.colorsOfVertices[l.vertices[1].colorId-1]->b = V1_prev_b + (V1_prev_b-V0_prev_b) * colorMultiplier;
							}if (tE > 0) {
                                l.vertices[0].x = x0 + dx * tE;
                                l.vertices[0].y = y0 + dy * tE;
                                l.vertices[0].z = z0 + dx * tE;
                                if(x1 - x0!=0)  colorMultiplier= (l.vertices[0].x-x1)/(x1 - x0);
                                else if(y1 - y0!=0)  colorMultiplier= (l.vertices[0].y-y1)/(y1 - y0);
                                else if (z1 - z0!=0)  colorMultiplier= (l.vertices[0].z-z1)/(z1 - z0);
                                scene.colorsOfVertices[l.vertices[0].colorId-1]->r = V1_prev_r + (V1_prev_r-V0_prev_r) * colorMultiplier;
                                scene.colorsOfVertices[l.vertices[0].colorId-1]->g = V1_prev_g + (V1_prev_g-V0_prev_g) * colorMultiplier;
                                scene.colorsOfVertices[l.vertices[0].colorId-1]->b = V1_prev_b + (V1_prev_b-V0_prev_b) * colorMultiplier;
							}
							return true;
						}
	return false; // TODO: not visible what to do set null,s add field etc ?
}

vector<generated_triangle> CameraHandler::apply_clipping(generated_triangle& m)
{
	vector<Vec4> old_v, new_v;
	old_v.push_back(m.vertices[0]);
	old_v.push_back(m.vertices[1]);
	old_v.push_back(m.vertices[2]);
	float tmax = m.vertices[0].t;
	float tmin = -1 * m.vertices[0].t;
	for (int axis = 0; axis < 3; axis++) {
		for (int i = 0; i < old_v.size() - 1; i++) {
			if (old_v[i].getElementAt(axis) <= tmax) {
				if (old_v[i + 1].getElementAt(axis) <= tmax) {
					new_v.push_back(old_v[i + 1]);
				}
				else if (old_v[i + 1].getElementAt(axis) > tmax) {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = tmax - old_v[i].getElementAt(axis);

                    Vec4 temp = slope * diff;
					Vec4 vip = old_v[i] + temp;
					new_v.push_back(vip);
				}
			}
			else {
				if (old_v[i + 1].getElementAt(axis) > tmax) { //TODO not sure about equality ?

				}
				else {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = tmax - old_v[i].getElementAt(axis);

                    Vec4 temp = slope * diff;
					Vec4 vip = old_v[i] + temp;
					new_v.push_back(vip);
					new_v.push_back(old_v[i + 1]);
				}
			}
		}
		int i = old_v.size() - 1;
		if (old_v[i].getElementAt(axis) <= tmax) {
			if (old_v[0].getElementAt(axis) <= tmax) {
				new_v.push_back(old_v[0]);
			}
			else if (old_v[i + 1].getElementAt(axis) > tmax) {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = tmax - old_v[i].getElementAt(axis);

                Vec4 temp = slope * diff;
				Vec4 vip = old_v[i] + temp;
				new_v.push_back(vip);
			}
		}
		else {
			if (old_v[0].getElementAt(axis) > tmax) { //TODO not sure about equality ?

			}
			else {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = tmax - old_v[i].getElementAt(axis);

                Vec4 temp = slope * diff;
				Vec4 vip = old_v[i] + temp;
				new_v.push_back(vip);
				new_v.push_back(old_v[0]);
			}
		}
		old_v = move(new_v);

		// iteration for maxs
		for (int i = 0; i < old_v.size() - 1; i++) {
			if (old_v[i].getElementAt(axis) >= tmin) {
				if (old_v[i + 1].getElementAt(axis) >= tmin) {
					new_v.push_back(old_v[i + 1]);
				}
				else if (old_v[i + 1].getElementAt(axis) < tmin) {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = tmin - old_v[i].getElementAt(axis);

                    Vec4 temp = slope * diff;
					Vec4 vip = old_v[i] + temp;
					new_v.push_back(vip);
				}
			}
			else {
				if (old_v[i + 1].getElementAt(axis) < tmin) { //TODO not sure about equality ?

				}
				else {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = tmin - old_v[i].getElementAt(axis);

                    Vec4 temp = slope * diff;
					Vec4 vip = old_v[i] + temp;
					new_v.push_back(vip);
					new_v.push_back(old_v[i + 1]);
				}
			}
		}
		i = old_v.size() - 1;
		if (old_v[i].getElementAt(axis) >= tmin) {
			if (old_v[0].getElementAt(axis) >= tmin) {
				new_v.push_back(old_v[0]);
			}
			else if (old_v[i + 1].getElementAt(axis) < tmin) {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = tmin - old_v[i].getElementAt(axis);

                Vec4 temp = slope * diff;
				Vec4 vip = old_v[i] + temp;
				new_v.push_back(vip);
			}
		}
		else {
			if (old_v[0].getElementAt(axis) < tmin) { //TODO not sure about equality ?

			}
			else {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = tmin - old_v[i].getElementAt(axis);

                Vec4 temp = slope * diff;
				Vec4 vip = old_v[i] + temp;
				new_v.push_back(vip);
				new_v.push_back(old_v[0]);
			}
		}
		old_v = move(new_v);
	}
	vector <generated_triangle> new_triangles;
	for (int i = 0; i < new_v.size() - 2; i++) {
		generated_triangle t(old_v[i],old_v[i+1],old_v[i+2]);
		new_triangles.push_back(t);
	}
	return new_triangles;
}

void CameraHandler::render()
{
	generate_cameraTrans_matrix();
	generate_orthographic_matrix();
	generate_perspective_matrix();
	for (Mesh* mesh : scene.meshes) {
		GeneratedMesh m(*mesh, scene);
		apply_modeling_transformation(m);
		//apply_clipping(m);
		//apply_culling(m);
		apply_viewing_transformations(m);
		render(m);
	}
}

void CameraHandler::render(GeneratedMesh& m)
{
	if (m.original.type == 0) {
		for (generated_line& l : m.generated_lines) render(l);
	}
	else if (m.original.type == 1) {
		for (generated_triangle& t : m.generated_triangles) render(t);
	}
}

void CameraHandler::render(generated_triangle& t)
{
	// Applies rasterization to triangle
	int xmin, ymin, xmax, ymax,x,y;
	xmin = min(min(int(t.vertices[0].x), int(t.vertices[1].x)), int(t.vertices[2].x));
	ymin = min(min(int(t.vertices[0].y), int(t.vertices[1].y)), int(t.vertices[2].y));
	xmax = max(max(int(t.vertices[0].x), int(t.vertices[1].x)), int(t.vertices[2].x));
	ymax = max(max(int(t.vertices[0].y), int(t.vertices[1].y)), int(t.vertices[2].y));
	float f01, f12, f20, a, b, c;
	auto f = [](const float x, const float y, const float x0, const float y0, const float x1, const float y1) {
		return x*(y0-y1)+y*(x1-x0)+x0*y1-y0*x1;
	};
	f01 = f(t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y);
	f12 = f(t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y);
	f20 = f(t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y);
	Color col;
	Color c0 = *scene.colorsOfVertices[t.vertices[0].colorId-1];
	Color c1 = *scene.colorsOfVertices[t.vertices[1].colorId-1];
	Color c2 = *scene.colorsOfVertices[t.vertices[2].colorId-1];

	for (y = ymin; y <= ymax;y++) {
		for (x = xmin; x <= xmax; x++) {
			a = f(x,y, t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y)/f12;
			if (a < 0) continue;
			b = f(x,y, t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y)/f20;
			if (b < 0) continue;
			c = f(x,y, t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y)/f01;
			if (c < 0) continue;
			col.r = a * c0.r + b * c1.r + c * c2.r;
			col.g = a * c0.g + b * c1.g + c * c2.g;
			col.b = a * c0.b + b * c1.b + c * c2.b;
			image[y][x] = col;
		}
	}


}

void CameraHandler::render(generated_line& l)
{
	int x, y,x0,y0,x1,y1,d;
	Color c;
	Color dc;
	float x_diff = (l.vertices[1].x - l.vertices[0].x);
	float y_diff = (l.vertices[1].y - l.vertices[0].y);
	float slope;
	if (x_diff == 0) slope = 2;
	else if (y_diff == 0 && x_diff < 0) slope = -0.1;
	else if (y_diff == 0 && x_diff > 0) slope = 0.1;
	else slope = y_diff / x_diff;
	if (slope < 1 && slope > -1) {
		if (slope > 0) {
			y0 = l.vertices[0].y;
			y1 = l.vertices[1].y;
			x0 = l.vertices[0].x;
			x1 = l.vertices[1].x;
			c = *scene.colorsOfVertices[l.vertices[0].colorId-1];
			dc.r = (scene.colorsOfVertices[l.vertices[1].colorId-1]->r - c.r )/ (x1 - x0);
			dc.g = (scene.colorsOfVertices[l.vertices[1].colorId-1]->g - c.g )/ (x1 - x0);
			dc.b = (scene.colorsOfVertices[l.vertices[1].colorId-1]->b - c.b )/ (x1 - x0);
		}
		else {
			y0 = l.vertices[1].y;
			y1 = l.vertices[0].y;
			x0 = l.vertices[1].x;
			x1 = l.vertices[0].x;
			c = *scene.colorsOfVertices[l.vertices[1].colorId-1];
			dc.r = (scene.colorsOfVertices[l.vertices[0].colorId-1]->r - c.r) / (x1 - x0);
			dc.g = (scene.colorsOfVertices[l.vertices[0].colorId-1]->g - c.g) / (x1 - x0);
			dc.b = (scene.colorsOfVertices[l.vertices[0].colorId-1]->b - c.b) / (x1 - x0);
		}
		y = y0;
		x = x0;
		d = 2 * (y0 - y1) + (x1 - x0);
		while (x <= x1) {
			image[y][x] = c;
			if (d < 0) {
				y = y + 1;
				d += 2 * (y0 - y1 + x1 - x0);
			}
			else {
				d += 2 * (y0 - y1);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
			x++;
		}
	}
	else {
		if (slope > 0) {
			y0 = l.vertices[0].y;
			y1 = l.vertices[1].y;
			x0 = l.vertices[0].x;
			x1 = l.vertices[1].x;
			c = *scene.colorsOfVertices[l.vertices[0].colorId-1];
			dc.r = (scene.colorsOfVertices[l.vertices[1].colorId-1]->r - c.r )/ (y1 - y0);
			dc.g = (scene.colorsOfVertices[l.vertices[1].colorId-1]->g - c.g )/ (y1 - y0);
			dc.b = (scene.colorsOfVertices[l.vertices[1].colorId-1]->b - c.b )/ (y1 - y0);
		}
		else {
			y0 = l.vertices[1].y;
			y1 = l.vertices[0].y;
			x0 = l.vertices[1].x;
			x1 = l.vertices[0].x;
			c = *scene.colorsOfVertices[l.vertices[1].colorId-1];
			dc.r = (scene.colorsOfVertices[l.vertices[0].colorId-1]->r - c.r) / (y1 - y0);
			dc.g = (scene.colorsOfVertices[l.vertices[0].colorId-1]->g - c.g) / (y1 - y0);
			dc.b = (scene.colorsOfVertices[l.vertices[0].colorId-1]->b - c.b) / (y1 - y0);
		}
		y = y0;
		x = x0;
		d = 2 * (x0 - x1) + (y1 - y0);
		while (y <= y1) {
			image[y][x] = c;
			if (d < 0) {
				x = x + 1;
				d += 2 * (x0 - x1 + y1 - y0);
			}
			else {
				d += 2 * (x0 - x1);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
			y++;
		}
	}
	
	// Applies rasterization to line
}

CameraHandler::CameraHandler(Camera& camera_, Scene& scene_):camera(camera_),scene(scene_),image(scene_.image)
{
    // -------- HANDY VARS -----------
    r = this->camera.right;
    l = this->camera.left;
    t = this->camera.top;
    b = this->camera.bottom;
    f = this->camera.far;
    n = this->camera.near;
    // ------------ RESOLUTION ---------
    nx = this->camera.horRes;
    ny = this->camera.verRes;
    generate_cameraTrans_matrix();
	generate_orthographic_matrix();
	generate_perspective_matrix();
	if (camera.projectionType == 1) {
		viewingTrans = perspective;
	}
	else {
		viewingTrans = orthographic;
	}
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
