#pragma once

#include "Vector3.h"

struct Vertex
{
	//float _x, _y;
	//float _r, _g, _b;


public:
	Vector3 _position;
	Vector3 _color;
	Vector3 _normal;
	Vector2 _textcoords;
	Vertex();
	Vertex(Vector3 pos, Vector3 normal, Vector2 text);
	//VertexCustomClass(float x, float y, float r, float g, float b);
	//~VertexCustomClass();
};

