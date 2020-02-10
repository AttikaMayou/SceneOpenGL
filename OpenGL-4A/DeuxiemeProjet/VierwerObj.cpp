#include <iostream>
using namespace std;

#include "tiny_obj_loader.h"

#include "ViewerObj.h"


void ViewerObj::LoadObj(Mesh &mesh) {

    std::string inputfile = "cubeBlender.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

   /* if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }*/

    if (!ret) {
        exit(1);
    }


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;

        mesh.setSize(shapes[s].mesh.indices.size());

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex

                Vertex vertex;
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vertex._position._x = attrib.vertices[3 * idx.vertex_index + 0];
                 vertex._position._y = attrib.vertices[3 * idx.vertex_index + 1];
                 vertex._position._z = attrib.vertices[3 * idx.vertex_index + 2];
                 if (attrib.normals.size() != 0) {
                    vertex._normal._x = attrib.normals[3 * idx.normal_index + 0];
                    vertex._normal._y = attrib.normals[3 * idx.normal_index + 1];
                    vertex._normal._z = attrib.normals[3 * idx.normal_index + 2];
                 }
                 if (attrib.texcoords.size() != 0) {
                     vertex._textcoords._x = attrib.texcoords[2 * idx.texcoord_index + 0];
                     vertex._textcoords._y = attrib.texcoords[2 * idx.texcoord_index + 1];
                 }

				 vertex._color._x = attrib.colors[3 * idx.vertex_index + 0];
				 vertex._color._y = attrib.colors[3 * idx.vertex_index + 1];
				 vertex._color._z = attrib.colors[3 * idx.vertex_index + 2];
				 
                 mesh.addVertex(vertex);
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}



