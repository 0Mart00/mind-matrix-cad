```markdown
# Mind-Matrix-CAD (MMC)

A highly modular, scalable, and lightweight CAD application written in pure C (C99). It is designed for creating, transforming, animating, and real-time projecting of arbitrary $N$-dimensional geometries. The software features a dynamic runtime C-scripting subsystem (via `.so` / `.dll` hot-reloading) and a mathematical formula evaluation pipeline.

## Key Features

*   **Arbitrary $N$-Dimensional Space:** Full support for hyper-dimensional vector operations, $N$-dimensional homogeneous transformation matrices, and plane-based hyper-rotations.
*   **Dynamic C-Scripting (Hot-Reloading):** Write custom geometric primitives in pure C. The system compiles and injects them into the engine at runtime without restarting the application.
*   **Hybrid Geometry Engine:** Supports multiple geometric representations natively, including Parametric $N$-D Surfaces and Implicit SDFs (Signed Distance Fields).
*   **Decoupled Architecture:** Strict separation of concerns across dedicated modules (Core, Math, Geometry, Render, UI, Scripting) to allow easy porting to other graphics APIs (e.g., Vulkan, WebGPU) or multi-threaded CPU acceleration.
*   **Immediate Mode UI:** An intuitive sidebar properties panel for manipulating object dimensions, transformations, and live script inputs.

---

## Repository Structure

The project employs a highly scalable directory structure tailored for robust build management and SDK isolation:

```text
n_cad/
├── CMakeLists.txt              # Modern CMake build configuration
├── apps/
│   └── main_app/
│       └── main.c              # Application entry point and main loop initialization
├── src/
│   ├── core/                   # Engine runtime, event handling, and timing
│   │   ├── app.c
│   │   └── engine.c
│   ├── math/                   # N-Dimensional linear algebra and projections
│   │   ├── vector_nd.c
│   │   ├── matrix_nd.c
│   │   └── projection.c
│   ├── geometry/               # Topology, mesh structures, and representation types
│   │   ├── geometry_base.c     # Common geometric abstract interfaces
│   │   ├── parametric.c        # Parametric N-D surface generator
│   │   ├── sdf.c               # Implicit Signed Distance Field generator
│   │   └── mesh_nd.c           # Flattened N-D vertex arrays and memory management
│   ├── render/                 # Graphics pipeline (OpenGL abstraction)
│   │   ├── renderer.c          # Framebuffer operations and draw loops
│   │   ├── shader.c            # GLSL shader compilation and binding
│   │   └── camera.c            # 3D and N-D view/projection matrices
│   ├── ui/                     # Immediate Mode GUI panels
│   │   ├── gui_manager.c       # UI context state and backend binding
│   │   ├── panel_properties.c  # Sidebar inspector panel for mesh modifications
│   │   └── panel_console.c     # Live output window for compilation errors and logs
│   └── scripting/              # Runtime compilation and dynamic loading
│       ├── compiler_bridge.c   # Async system wrapper for GCC/Clang invocations
│       ├── plugin_manager.c    # OS-specific shared library (.so/.dll) handles
│       └── formula_parser.c    # Runtime mathematical string parser (TinyExpr wrapper)
├── include/                    # Public header files mirroring the src/ hierarchy
│   ├── core/      ├── math/       ├── geometry/
│   ├── render/    ├── ui/         └── scripting/
├── shaders/                    # GLSL source code
│   ├── vertex_nd.glsl          # Core shader handling high-dimensional space compression
│   └── fragment.glsl
├── sdk/                        # Software Development Kit for custom scripts
│   └── ncad_sdk.h              # Unified header exposed to external plugins
├── scripts/                    # User-defined geometry scripts (Hot-reload targets)
│   ├── hyper_torus.c
│   └── custom_wave.c
└── third_party/                # Embedded header-only/submodule third-party libraries
    ├── glfw/                   # Window and input context
    ├── nuklear/                # Pure ANSI C immediate-mode GUI
    └── tinyexpr/               # Fast mathematical string evaluation engine

```

---

## Technical Stack & Dependencies

The external dependency footprint is deliberately restricted to lightweight, pure-C compatible libraries:

| Subsystem | Component / Library | Responsibility | License |
| --- | --- | --- | --- |
| **Language** | C99 Standard | Base system language | - |
| **Graphics API** | OpenGL 3.3+ (Core Profile) | Hardware acceleration and rasterization | Varies |
| **Windowing** | GLFW 3 | Cross-platform window and context management | zlib/libpng |
| **GL Loading** | Glad / GLEW | Runtime OpenGL function pointer resolution | MIT / BSD |
| **UI Engine** | Nuklear | Single-header, zero-allocation Immediate Mode GUI | Public Domain |
| **Math Parser** | TinyExpr | Thread-safe arithmetic string runtime evaluation | zlib |
| **OS Interface** | POSIX `dlfcn` / WinAPI | Dynamically loaded library orchestration | Native |

---

## Processing Pipeline

```
[ User Script / Formula ] ---> Generates N-Dimensional MeshND (Flat Array)
                                        │
                                        ▼
[ Math Module ] -------------> Applies N-D Homogeneous Transformations (Rotation/Translation)
                                        │
                                        ▼
[ Projection Module ] --------> Cascades Dimensions Successively: N-D -> (N-1)-D -> ... -> 3D
                                        │
                                        ▼
[ Render Module ] ------------> Batches 3D Vertices to GPU buffers and draws via GLSL Shaders

```

---

## Getting Started

### Prerequisites (Ubuntu / Debian Example)

Ensure a C99-compliant compiler (GCC or Clang), CMake, and development packages for graphics contexts are present on your machine:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev libglew-dev

```

### Building the Project

Mind-Matrix-CAD (MMC) utilizes CMake for cross-platform out-of-source builds:

```bash
mkdir build
cd build
cmake ..
cmake --build .

```

### Running the Application

Execute the compiled binary from the build root directory:

```bash
./apps/main_app/n_cad

```

---

## Creating Custom Geometry Scripts

To extend the application with custom hyper-dimensional geometry, implement a plugin module using the public SDK header `ncad_sdk.h`.

Save your source code in the `scripts/` folder (e.g., `scripts/my_geometry.c`):

```c
#include <ncad_sdk.h>
#include <math.h>

// Specific data structure for tracking your custom parameters
typedef struct {
    double frequency;
    double amplitude;
} WaveData;

// Executed on the CPU every frame before rendering
void CustomUpdate(Geometry *geom, double t) {
    WaveData *data = (WaveData*)geom->internal_data;
    // Animate structural parameters over running time (t)
    data->amplitude = 1.5 + sin(t);
}

// Generates the N-dimensional vertex array topology
void CustomGenerateMesh(Geometry *geom, MeshND *out_mesh) {
    WaveData *data = (WaveData*)geom->internal_data;
    // Algorithmic allocation and mapping of higher-dimensional vertices...
}

void CustomFree(Geometry *geom) {
    free(geom->internal_data);
    free(geom);
}

// Module entry point called by the CAD's dynamic loader
Geometry* InitCustomGeometry() {
    Geometry *geom = malloc(sizeof(Geometry));
    WaveData *data = malloc(sizeof(WaveData));
    
    data->frequency = 2.0;
    data->amplitude = 1.0;
    
    geom->internal_data = data;
    geom->vtable.update = CustomUpdate;
    geom->vtable.generate_mesh = CustomGenerateMesh;
    geom->vtable.free = CustomFree;
    
    return geom;
}

```

### Loading your Plugin

1. Open Mind-Matrix-CAD (MMC) and navigate to the **Properties Panel** on the side.
2. Click **"Load Script Source"** and choose your `.c` file.
3. The internal `compiler_bridge` compiles the target to a shared library binary `.so`/`.dll` silently in a worker thread, signals the engine to cleanly free the previous layout, and live-loads the new entity into the active simulation context.

```

```
