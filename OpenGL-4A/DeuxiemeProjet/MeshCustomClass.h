#pragma once

#include "VertexCustomClass.h"

struct Mesh {
private:
	Vertex* _vertices;
	int _vertexCount;

public:
	Mesh();
	~Mesh();
	void setSize(int i);
	void addVertex(Vertex& vertex);
	Vertex* getVertices();
	int getVertexCount();
};
