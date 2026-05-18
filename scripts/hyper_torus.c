#include "../include/sdk/ncad_sdk.h"  // Az SDK beimportálása
#include <stdlib.h>
#include <math.h>

// 1. Saját belső adatszerkezet (a főprogram nem tudja mi ez, nem is érdekli)
typedef struct {
    double radius_1;
    double radius_2;
    double speed;
} HyperTorusData;

// 2. Az UPDATE függvény: Minden képkockában lefut, itt számolunk animációt
void CustomUpdate(Geometry *geom, double t) {
    HyperTorusData *data = (HyperTorusData*)geom->internal_data;
    
    // Az ADHD/Savant barát vezérlés: az idő függvényében pulzáltatjuk a belső sugarat
    data->radius_2 = 1.0 + sin(t * data->speed) * 0.5;
}

// 3. A MESH GENERÁTOR: Itt töltjük fel az N-dimenziós koordináta-tömböt
void CustomGenerateMesh(Geometry *geom, MeshND *out_mesh) {
    HyperTorusData *data = (HyperTorusData*)geom->internal_data;
    
    // Példaként generáljunk egy nagyon egyszerű 5D-s hálót (2 pont, 1 él)
    out_mesh->dim = 5;
    out_mesh->num_vertices = 2;
    out_mesh->vertices = (double*)malloc(out_mesh->num_vertices * out_mesh->dim * sizeof(double));
    
    // 1. Pont koordinátái (5D)
    out_mesh->vertices[0] = data->radius_1; // X
    out_mesh->vertices[1] = 0.0;            // Y
    out_mesh->vertices[2] = 0.0;            // Z
    out_mesh->vertices[3] = data->radius_2; // W (4D)
    out_mesh->vertices[4] = 1.0;            // V (5D)
    
    // 2. Pont koordinátái (5D)
    out_mesh->vertices[5] = -data->radius_1;
    out_mesh->vertices[6] = 0.0;
    out_mesh->vertices[7] = 0.0;
    out_mesh->vertices[8] = -data->radius_2;
    out_mesh->vertices[9] = -1.0;

    // Élek összekötése
    out_mesh->num_edges = 1;
    out_mesh->edges = (int*)malloc(out_mesh->num_edges * 2 * sizeof(int));
    out_mesh->edges[0] = 0; // 0. pont
    out_mesh->edges[1] = 1; // összekötve az 1. ponttal
}

// 4. MEMÓRIA FELSZABADÍTÁS: Ha a scriptet bezárjuk vagy újraforgatjuk
void CustomFree(Geometry *geom) {
    free(geom->internal_data);
    free(geom);
}

// ====================================================================
// FONTOS: Ez a belépési pont! Ezt a nevet fogja keresni a főprogram!
// ====================================================================
Geometry* InitCustomGeometry() {
    Geometry *geom = (Geometry*)malloc(sizeof(Geometry));
    HyperTorusData *data = (HyperTorusData*)malloc(sizeof(HyperTorusData));
    
    // Alapértelmezett értékek beállítása
    data->radius_1 = 3.0;
    data->radius_2 = 1.0;
    data->speed = 2.5;
    
    geom->internal_data = data;
    
    // Függvénymutatók bekötése a VTable-be
    geom->vtable.update = CustomUpdate;
    geom->vtable.generate_mesh = CustomGenerateMesh;
    geom->vtable.free_data = CustomFree;
    
    return geom;
}
