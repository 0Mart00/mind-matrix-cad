#include "scripting/plugin_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

PluginContext LoadGeometryPlugin(const char *source_c_path) {
    PluginContext ctx = {NULL, NULL};
    char compile_cmd[512];
    const char *output_so = "./scripts/runtime_output.so";

    // Összeállítjuk a háttér-fordító parancsot (GCC)
    // -shared: Megosztott könyvtár, -fPIC: Pozíciófüggetlen kód (kötelező .so-hoz)
    sprintf(compile_cmd, "gcc -shared -fPIC -O2 %s -o %s", source_c_path, output_so);
    
    printf("[COMPILER]: Compiling script: %s...\n", source_c_path);
    int compile_result = system(compile_cmd);
    
    if (compile_result != 0) {
        fprintf(stderr, "[COMPILER ERROR]: Failed to compile %s\n", source_c_path);
        return ctx; // Ha hibás a kód, üres kontextust adunk vissza, nem omlik össze a CAD
    }

    // Dinamikus könyvtár megnyitása a memóriában
    ctx.lib_handle = dlopen(output_so, RTLD_NOW);
    if (!ctx.lib_handle) {
        fprintf(stderr, "[DYNAMIC LOADER ERROR]: %s\n", dlerror());
        return ctx;
    }

    // Megkeressük a scriptben megírt kötelező 'InitCustomGeometry' függvényt
    Geometry* (*InitFunc)(void) = dlsym(ctx.lib_handle, "InitCustomGeometry");
    if (!InitFunc) {
        fprintf(stderr, "[DYNAMIC LOADER ERROR]: Initialization function symbol not found!\n");
        dlclose(ctx.lib_handle);
        ctx.lib_handle = NULL;
        return ctx;
    }

    // Példányosítjuk a geometriát a scriptből kapott függvénymutatókkal
    ctx.geometry = InitFunc();
    printf("[SYSTEM]: Plugin loaded and activated successfully.\n");

    return ctx;
}

void UnloadGeometryPlugin(PluginContext *ctx) {
    if (ctx->geometry) {
        // Meghívjuk a script saját memóriafelszabadítóját a vtable-ből
        if (ctx->geometry->vtable.free_data) {
            ctx->geometry->vtable.free_data(ctx->geometry);
        }
        ctx->geometry = NULL;
    }
    if (ctx->lib_handle) {
        dlclose(ctx->lib_handle);
        ctx->lib_handle = NULL;
    }
}
