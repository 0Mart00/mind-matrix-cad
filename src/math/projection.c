#include "math/projection.h"
#include <stdlib.h>
#include <string.h>

void ProjectMeshNDTo3D(const MeshND *in_mesh, float *out_vertices_3d, const double *camera_distances) {
    int dim = in_mesh->dim;
    double *current_vertex = (double*)malloc(dim * sizeof(double));

    for (int v = 0; v < in_mesh->num_vertices; v++) {
        // Aktuális pont beolvasása a lapított tömbből
        memcpy(current_vertex, &in_mesh->vertices[v * dim], dim * sizeof(double));
        
        // Dimenziók csökkentése ciklussal, amíg el nem érjük a 3D-t
        // Pl. 5D -> 4D -> 3D
        int current_dim = dim;
        while (current_dim > 3) {
            int drop_axis = current_dim - 1; // Az utolsó tengely koordinátája (pl. 5. tengely)
            double d = camera_distances[current_dim - 4]; // Kameratávolság ehhez a dimenzióhoz
            
            double factor = 1.0 / (1.0 - (current_vertex[drop_axis] / d));
            
            // Minden megmaradó koordinátát skálázunk a perspektíva miatt
            for (int i = 0; i < drop_axis; i++) {
                current_vertex[i] *= factor;
            }
            current_dim--;
        }
        
        // Az eredményül kapott X, Y, Z koordinátákat float tömbbé alakítva átadjuk az OpenGL-nek
        out_vertices_3d[v * 3 + 0] = (float)current_vertex[0];
        out_vertices_3d[v * 3 + 1] = (float)current_vertex[1];
        out_vertices_3d[v * 3 + 2] = (float)current_vertex[2];
    }

    free(current_vertex);
}
