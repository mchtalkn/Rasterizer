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

bool CameraHandler::backface_culling(Triangle& t)
{
	return false;
}

GeneratedMesh& CameraHandler::apply_clipping(GeneratedMesh& m)
{
	// TODO:
	if (m.original.type == 0) {
		vector<generated_triangle> new_t;
		for (int i = 0; i < m.generated_triangles.size(); i++) {
			vector<generated_triangle> append = apply_clipping(m.generated_triangles[i]);
			new_t.insert(new_t.end(), append.begin(), append.end());
		}
	}
	if (m.original.type == 1) {
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
	vector<Vec4> old_v, new_v;
	old_v.push_back(m.vertices[0]);
	old_v.push_back(m.vertices[1]);
	old_v.push_back(m.vertices[2]);
	float mins[3] = { xmin,ymin,zmin };
	float maxs[3] = { xmax,ymax,zmax };
	for (int axis = 0; axis < 3; axis++) {
		for (int i = 0; i < old_v.size() - 1; i++) {
			if (old_v[i].getElementAt(axis) <= maxs[axis]) {
				if (old_v[i + 1].getElementAt(axis) <= maxs[axis]) {
					new_v.push_back(old_v[i + 1]);
				}
				else if (old_v[i + 1].getElementAt(axis) > maxs[axis]) {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = maxs[axis] - old_v[i].getElementAt(axis);

					Vec4 vip = old_v[i] + slope * diff;
					new_v.push_back(vip);
				}
			}
			else {
				if (old_v[i + 1].getElementAt(axis) > maxs[axis]) { //TODO not sure about equality ?

				}
				else {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = maxs[axis] - old_v[i].getElementAt(axis);

					Vec4 vip = old_v[i] + slope * diff;
					new_v.push_back(vip);
					new_v.push_back(old_v[i + 1]);
				}
			}
		}
		int i = old_v.size() - 1;
		if (old_v[i].getElementAt(axis) <= maxs[axis]) {
			if (old_v[0].getElementAt(axis) <= maxs[axis]) {
				new_v.push_back(old_v[0]);
			}
			else if (old_v[i + 1].getElementAt(axis) > maxs[axis]) {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = maxs[axis] - old_v[i].getElementAt(axis);

				Vec4 vip = old_v[i] + slope * diff;
				new_v.push_back(vip);
			}
		}
		else {
			if (old_v[0].getElementAt(axis) > maxs[axis]) { //TODO not sure about equality ?

			}
			else {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = maxs[axis] - old_v[i].getElementAt(axis);

				Vec4 vip = old_v[i] + slope * diff;
				new_v.push_back(vip);
				new_v.push_back(old_v[0]);
			}
		}
		old_v = move(new_v);

		// iteration for maxs
		for (int i = 0; i < old_v.size() - 1; i++) {
			if (old_v[i].getElementAt(axis) >= mins[axis]) {
				if (old_v[i + 1].getElementAt(axis) >= mins[axis]) {
					new_v.push_back(old_v[i + 1]);
				}
				else if (old_v[i + 1].getElementAt(axis) < mins[axis]) {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = mins[axis] - old_v[i].getElementAt(axis);

					Vec4 vip = old_v[i] + slope * diff;
					new_v.push_back(vip);
				}
			}
			else {
				if (old_v[i + 1].getElementAt(axis) < mins[axis]) { //TODO not sure about equality ?

				}
				else {
					Vec4 slope = old_v[i + 1] - old_v[i];
					float diff = mins[axis] - old_v[i].getElementAt(axis);

					Vec4 vip = old_v[i] + slope * diff;
					new_v.push_back(vip);
					new_v.push_back(old_v[i + 1]);
				}
			}
		}
		int i = old_v.size() - 1;
		if (old_v[i].getElementAt(axis) >= mins[axis]) {
			if (old_v[0].getElementAt(axis) >= mins[axis]) {
				new_v.push_back(old_v[0]);
			}
			else if (old_v[i + 1].getElementAt(axis) < mins[axis]) {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = mins[axis] - old_v[i].getElementAt(axis);

				Vec4 vip = old_v[i] + slope * diff;
				new_v.push_back(vip);
			}
		}
		else {
			if (old_v[0].getElementAt(axis) < mins[axis]) { //TODO not sure about equality ?

			}
			else {
				Vec4 slope = old_v[0] - old_v[i];
				float diff = mins[axis] - old_v[i].getElementAt(axis);

				Vec4 vip = old_v[i] + slope * diff;
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
	for (GeneratedMesh& m : generated_meshes) render(m);
	//TODO write image to file
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
}

void CameraHandler::render(generated_line& l)
{
	// Applies rasterization to line
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
