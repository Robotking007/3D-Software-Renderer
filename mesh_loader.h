#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "math_utils.h"

typedef struct {
    Vec3* vertices;
    Vec3* normals;
    Vec3* uvs;
    int* indices;
    int vertex_count;
    int index_count;
} Mesh;

// Mesh management
void load_cube(Mesh* mesh);
void free_mesh(Mesh* mesh);

#endif // MESH_LOADER_H