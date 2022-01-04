#include "Transformation.h"

Transformation::Transformation(const Scaling& s)
{
    double val[4][4] = {[s.sx, 0 , 0 , 0],[0, s.sy , 0 , 0],[0, 0 , s.sz , 0],[0, 0 , 0 , 1] };
    this->matrix =  Matrix4(val);
}

Transformation::Transformation(const Rotation& r)
{
}

Transformation::Transformation(const Translation& t)
{
}

Transformation& Transformation::concatenate(Transformation& t)
{
	// TODO: insert return statement here
	return t;
}

Mesh& Transformation::apply(Mesh& m)
{
	// TODO: insert return statement here
	return m;
}

Triangle& Transformation::apply(Triangle& t)
{
	// TODO: insert return statement here
	return t;
}

Vec4& Transformation::apply(Vec4& v)
{
	// TODO: insert return statement here
	return v;
}
