#include "Transformation.h"

Transformation::Transformation(const Scaling& s)
{
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
