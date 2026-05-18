#ifndef MMC_PLUGIN_MANAGER_H
#define MMC_PLUGIN_MANAGER_H

#include "sdk/ncad_sdk.h"

typedef struct {
    void *lib_handle; // A betöltött .so fájl memóriacíme
    Geometry *geometry;
} PluginContext;

PluginContext LoadGeometryPlugin(const char *source_c_path);
void UnloadGeometryPlugin(PluginContext *ctx);

#endif
