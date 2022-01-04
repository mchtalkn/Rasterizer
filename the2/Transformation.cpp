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

GeneratedMesh& Transformation::apply(GeneratedMesh& m)
{
	// TODO: insert return statement here
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
