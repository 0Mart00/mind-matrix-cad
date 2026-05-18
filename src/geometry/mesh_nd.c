#include "geometry/mesh_nd.h"
#include <stdlib.h>

void AllocateMeshND(MeshND *mesh, int dim, int num_vertices, int num_edges) {
    mesh->dim = dim;
    mesh->num_vertices = num_vertices;
    mesh->num_edges = num_edges;
    mesh->vertices = (double*)malloc(num_vertices * dim * sizeof(double));
    mesh->edges = (int*)malloc(num_edges * 2 * sizeof(int));
}

void FreeMeshNDExplicit(MeshND *mesh) {
    if (mesh->vertices) {
        free(mesh->vertices);
        mesh->vertices = NULL;
    }
    if (mesh->edges) {
        free(mesh->edges);
        mesh->edges = NULL;
    }
    mesh->num_vertices = 0;
    mesh->num_edges = 0;
}
