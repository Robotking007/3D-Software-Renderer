#include "mesh_loader.h"
#include <stdlib.h>
#include <string.h>

void load_cube(Mesh* mesh) {
    Vec3 vertices[] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };
    
    int indices[] = {
        0,1,2, 0,2,3, 4,5,6, 4,6,7,
        0,3,7, 0,7,4, 1,2,6, 1,6,5,
        0,1,5, 0,5,4, 2,3,7, 2,7,6
    };
    
    mesh->vertices = malloc(sizeof(vertices));
    memcpy(mesh->vertices, vertices, sizeof(vertices));
    
    mesh->indices = malloc(sizeof(indices));
    memcpy(mesh->indices, indices, sizeof(indices));
    
    mesh->vertex_count = 8;
    mesh->index_count = 36;
    
    // Generate normals
    mesh->normals = malloc(mesh->vertex_count * sizeof(Vec3));
    for (int i = 0; i < mesh->vertex_count; i++) {
        mesh->normals[i] = vec3_normalize(mesh->vertices[i]);
    }
}

void free_mesh(Mesh* mesh) {
    free(mesh->vertices);
    free(mesh->indices);
    free(mesh->normals);
    free(mesh->uvs);
    memset(mesh, 0, sizeof(Mesh));
}