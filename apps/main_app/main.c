#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE  // Kényszerítjük a GLFW-t, hogy NE töltsön be semmilyen beépített OpenGL szemetet
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

// Az ADHD/Savant barát ablakméretek (fix, nem átméretezhető a zéró disztrakcióért)
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Hibakezelő függvény a GLFW-hez
void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "[GLFW Error %d]: %s\n", error, description);
}

int main(void) {
    // 1. Hibakezelés beállítása az inicializáció ELŐTT
    glfwSetErrorCallback(glfw_error_callback);
    
    // Arch Linux / Wayland / X11 automatikus hibrid platformkezelés kényszerítése
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL; 

    // 2. GLFW Inicializálása (Csak egyszer!)
    if (!glfwInit()) {
        fprintf(stderr, "Kritikus hiba: GLFW inicializálás sikertelen!\n");
        return EXIT_FAILURE;
    }

    // OpenGL 3.3 Core Profile ablak specifikációk
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Fix méret, zéró disztrakció

    // 3. Ablak létrehozása
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mind-Matrix-CAD // N-Dimensional Engine", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Kritikus hiba: Ablak létrehozása sikertelen!\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Kontextus aktívvá tétele az aktuális szálon
    glfwMakeContextCurrent(window);
   
    // 4. Glad 2 inicializálása (glfwGetProcAddress segítségével, típusmódosítás nélkül)
    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "Kritikus hiba: GLAD inicializálás sikertelen! (Nincs érvényes OpenGL kontextus)\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // V-Sync bekapcsolása (fix 60 FPS, hogy ne pörögje túl magát a GPU)
    glfwSwapInterval(1);

    printf("[SYSTEM]: Mind-Matrix-CAD sikeresen elindult GLAD backenddel.\n");
    printf("[RENDERER]: OpenGL verzió: %s\n", glGetString(GL_VERSION));

    // --- FŐ RENDERELÉSI HUROK ---
    while (!glfwWindowShouldClose(window)) {
        
        // Bemenetek (billentyűzet, egér) lekérdezése
        glfwPollEvents();

        // Háttér letisztítása (Sötét, agybarát mátrix-zöld szín)
        glClearColor(0.04f, 0.05f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- IDE JÖNNEK MAJD A MODULOK HÍVÁSAI ---
        // 1. Script update-ek futtatása (math/geometry)
        // 2. N-dimenziós mátrix forgatások és projekció számolása
        // 3. UI panel kirajzolása (Nuklear GUI)
        // 4. OpenGL Mesh rajzolás

        // Képfrissítés (Double Buffering)
        glfwSwapBuffers(window);
    }

    // 5. Erőforrások tiszta felszabadítása a bezáráskor
    printf("[SYSTEM]: Leállás, erőforrások felszabadítása...\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
