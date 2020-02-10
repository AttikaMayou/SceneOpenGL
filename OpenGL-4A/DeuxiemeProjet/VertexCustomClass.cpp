#include "VertexCustomClass.h"
#include "Vector3.h"

Vertex::Vertex() {
	_position = Vector3(0, 0, 0);
	_normal = Vector3(0, 0, 0);
	_textcoords = Vector2(0, 0);
}

Vertex::Vertex(Vector3 pos, Vector3 normal, Vector2 text) {
	_position = pos;
	_normal = normal;
	_textcoords = text;
}
