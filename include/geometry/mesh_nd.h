#ifndef MMC_MESH_ND_H
#define MMC_MESH_ND_H

#include "sdk/ncad_sdk.h"

void FreeMeshNDExplicit(MeshND *mesh);
void AllocateMeshND(MeshND *mesh, int dim, int num_vertices, int num_edges);

#endif
