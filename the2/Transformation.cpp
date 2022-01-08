#include "Transformation.h"

void swap( double &a,  double &b){
    double temp = a;
    a=b; b=temp;
}
Transformation::Transformation()
{
    double val[4][4] = { {1, 0 , 0 , 0 },{0, 1 , 0 , 0 },{0, 0 , 1 , 0 },{0, 0 , 0 , 1 } };
    this->matrix = Matrix4(val);
}
Transformation::Transformation(const Scaling& s)
{
    double val[4][4] = {{s.sx, 0 , 0 , 0 },{0, s.sy , 0 , 0 },{0, 0 , s.sz , 0 },{0, 0 , 0 , 1 } };
    this->matrix =  Matrix4(val);
}

Transformation::Transformation(const Rotation& r)
{
    Vec3 u(r.ux, r.ux, r.uz, 0);
    Vec3 v(0.0, 0.0, 0.0, 0);
    double smallest = min(min(ABS(r.ux), ABS(r.uy)), ABS(r.uz));
    if(smallest == ABS(r.ux)) { v.x=0.0; v.y = r.uz; v.z = -1*r.uy; }
    if(smallest == ABS(r.uy)) { v.y=0.0; v.x = r.uz; v.z = -1*r.ux; }
    if(smallest == ABS(r.uz)) { v.z=0.0; v.y = r.ux; v.x = -1*r.uy; }
    Vec3 w = crossProductVec3(u, v);
    normalizeVec3(v);
    normalizeVec3(w);
    double val[4][4] = {{u.x, v.x , w.x , 0},{u.y,  v.y , w.y , 0},{u.z,  v.z , w.z , 0},{0, 0 , 0 , 1}};
    double val2[4][4] = {{u.x, u.y, u.z ,  0},{v.x ,  v.y , v.z , 0},{w.x ,  w.y ,  w.z , 0},{0, 0 , 0 , 1}};
    double val3[4][4] = {{1,0,0,0},{0, cos(r.angle), -sin(r.angle), 0},{0, sin(r.angle), cos(r.angle),0}, {0,0,0,1}};
    Matrix4 M_inv =  Matrix4(val);
    Matrix4 M =  Matrix4(val2);
    Matrix4 M_rot =  Matrix4(val3);
    this->matrix = multiplyMatrixWithMatrix( M_inv, multiplyMatrixWithMatrix(M_rot, M));
}

Transformation::Transformation(const Translation& t)
{
    double val[4][4] = {{1,0,0, t.tx},{0, 1, 0, t.ty},{0, 0, 1, t.tz},{0, 0 , 0 , 1}};
    this->matrix =  Matrix4(val);
}

Transformation& Transformation::concatenate(const Transformation& t)
{
    this->matrix = multiplyMatrixWithMatrix(t.matrix, this->matrix);
	// TODO: insert return statement here
	return *this;
}

GeneratedMesh& Transformation::apply(GeneratedMesh& m)
{
    for (int i=0; i< m.generated_triangles.size() ; i++){
        for (int j=0 ; j<3; j++){
            // apply modeling transformation matrix in the object
            m.generated_triangles[i].vertices[j] = multiplyMatrixWithVec4(this->matrix, m.generated_triangles[i].vertices[j]);
        }
        // computing normal also makes t's all 1 beforehand.
        m.generated_triangles[i].normal = computeNormals(m.generated_triangles[i].vertices);
    }
	return m;
}

generated_triangle& Transformation::apply(generated_triangle& t)
{
	// TODO: insert return statement here
	return t;
}

Vec4& Transformation::apply(Vec4& v)
{
	// TODO: insert return statement here
	return v;
}
