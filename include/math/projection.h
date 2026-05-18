#ifndef MMC_PROJECTION_H
#define MMC_PROJECTION_H

#include "sdk/ncad_sdk.h"

// Levetít egy N-dimenziós pontfelhőt egy szabványos 3D-s pontfelhővé
// A 'camera_distances' egy tömb, ami a magasabb dimenziók kameratávolságait tartalmazza
void ProjectMeshNDTo3D(const MeshND *in_mesh, float *out_vertices_3d, const double *camera_distances);

#endif
