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
	Vec3 p1, p2, p3;
	Vec3 a,b,c;
	Vec3 n;
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
		i = old_v.size() - 1;
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
	int xmin, ymin, xmax, ymax,x,y;
	xmin = min(min(int(t.vertices[0].x), int(t.vertices[1].x)), int(t.vertices[3].x));
	ymin = min(min(int(t.vertices[0].y), int(t.vertices[1].y)), int(t.vertices[3].y));
	xmax = max(max(int(t.vertices[0].x), int(t.vertices[1].x)), int(t.vertices[3].x));
	ymax = max(max(int(t.vertices[0].y), int(t.vertices[1].y)), int(t.vertices[3].y));
	float f01, f12, f20, a, b, c;
	auto f = [](const float x, const float y, const float x0, const float y0, const float x1, const float y1) {
		return x*(y0-y1)+y*(x1-x0)+x0*y1-y0*x1;
	};
	f01 = f(t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y);
	f12 = f(t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y);
	f20 = f(t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y);
	Color col;
	Color c0 = *scene.colorsOfVertices[t.vertices[0].colorId];
	Color c1 = *scene.colorsOfVertices[t.vertices[1].colorId];
	Color c2 = *scene.colorsOfVertices[t.vertices[2].colorId];

	for (y = ymin; y <= ymax;y++) {
		for (x = xmin; x <= xmax; x++) {
			a = f(x,y, t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y);
			if (a < 0) continue;
			b = f(x,y, t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y);
			if (b < 0) continue;
			c = f(x,y, t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y);
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
	else slope = y_diff / x_diff;
	if (slope > 1 || slope < -1) {
		if (slope > 0) {
			y0 = l.vertices[0].y;
			y1 = l.vertices[1].y;
			x0 = l.vertices[0].x;
			x1 = l.vertices[1].x;
			c = *scene.colorsOfVertices[l.vertices[0].colorId];
			dc.r = (scene.colorsOfVertices[l.vertices[1].colorId]->r - c.r )/ (x1 - x0);
			dc.g = (scene.colorsOfVertices[l.vertices[1].colorId]->g - c.g )/ (x1 - x0);
			dc.b = (scene.colorsOfVertices[l.vertices[1].colorId]->b - c.b )/ (x1 - x0);
		}
		else {
			y0 = l.vertices[1].y;
			y1 = l.vertices[0].y;
			x0 = l.vertices[1].x;
			x1 = l.vertices[0].x;
			c = *scene.colorsOfVertices[l.vertices[1].colorId];
			dc.r = (scene.colorsOfVertices[l.vertices[0].colorId]->r - c.r) / (x1 - x0);
			dc.g = (scene.colorsOfVertices[l.vertices[0].colorId]->g - c.g) / (x1 - x0);
			dc.b = (scene.colorsOfVertices[l.vertices[0].colorId]->b - c.b) / (x1 - x0);
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
		}
	}
	else {
		if (slope > 0) {
			y0 = l.vertices[0].y;
			y1 = l.vertices[1].y;
			x0 = l.vertices[0].x;
			x1 = l.vertices[1].x;
			c = *scene.colorsOfVertices[l.vertices[0].colorId];
			dc.r = (scene.colorsOfVertices[l.vertices[1].colorId]->r - c.r )/ (y1 - y0);
			dc.g = (scene.colorsOfVertices[l.vertices[1].colorId]->g - c.g )/ (y1 - y0);
			dc.b = (scene.colorsOfVertices[l.vertices[1].colorId]->b - c.b )/ (y1 - y0);
		}
		else {
			y0 = l.vertices[1].y;
			y1 = l.vertices[0].y;
			x0 = l.vertices[1].x;
			x1 = l.vertices[0].x;
			c = *scene.colorsOfVertices[l.vertices[1].colorId];
			dc.r = (scene.colorsOfVertices[l.vertices[0].colorId]->r - c.r) / (y1 - y0);
			dc.g = (scene.colorsOfVertices[l.vertices[0].colorId]->g - c.g) / (y1 - y0);
			dc.b = (scene.colorsOfVertices[l.vertices[0].colorId]->b - c.b) / (y1 - y0);
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
		}
	}
	
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
