#include "MeshCustomClass.h"
#include "VertexCustomClass.h"

Mesh::Mesh() {
	_vertexCount = 0;
	_vertices = nullptr;
}

Mesh::~Mesh() {
	delete[] _vertices;
}

void Mesh::addVertex(Vertex& vertex) {
	_vertices[_vertexCount] = vertex;
	_vertexCount++;
}


void Mesh::setSize(int i) {
	_vertices = new Vertex[i];
}


Vertex* Mesh::getVertices() {
	return _vertices;
}

int Mesh::getVertexCount() {
	return _vertexCount;
}

