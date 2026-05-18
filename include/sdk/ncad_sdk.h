#ifndef NCAD_SDK_H
#define NCAD_SDK_H

// Lapított (Flat) N-dimenziós hálóstruktúra
typedef struct {
    int dim;            // Dimenziók száma (pl. 5)
    int num_vertices;   // Pontok száma
    double *vertices;   // Pontok koordinátái egymás után: [x1,y1,z1,w1,v1, x2,y2,z2,w2,v2, ...]
    int num_edges;      // Élek száma
    int *edges;         // Éllista (indexpárok): [0, 1, 1, 2, 2, 3, ...]
} MeshND;

// Előre deklarált Geometria struktúra
typedef struct Geometry Geometry;

// Interfész függyvénymutatókkal (VTable)
typedef struct {
    void (*update)(Geometry *geom, double t);
    void (*generate_mesh)(Geometry *geom, MeshND *out_mesh);
    void (*free_data)(Geometry *geom);
} GeometryInterface;

// A fő Geometria objektum
struct Geometry {
    void *internal_data;        // Egyedi adatok a scripten belül (bármi lehet)
    GeometryInterface vtable;   // A fenti függvénymutatók gyűjteménye
};

#endif // NCAD_SDK_H
