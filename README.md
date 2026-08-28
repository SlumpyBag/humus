# Humus - A Framework and Programming Language

![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Overview

**Humus** is a versatile framework and programming language written in C that combines the power of native code execution with modern features. Like Java, when you click a `.hes` (Humus Executable Script) file, it immediately runs off Humus instead of running in a web browser. However, Humus can also be embedded in web browsers for maximum flexibility.

### Key Features

- 🖥️ **Window Creation** - Cross-platform window management
- 🔘 **Button Creation** - Interactive UI elements with callbacks
- 📈 **Graphing Calculations** - Plot mathematical functions and data
- 🎮 **3D Calculations** - 3D transformations and projections
- ✨ **Ray Tracing** - Basic ray tracing engine for 3D rendering
- 🌐 **Web Interaction** - HTTP client for web APIs
- 💬 **P2P Chatroom** - Peer-to-peer networking capabilities
- 🎨 **Vector Image Support** - Create and export SVG graphics
- 🚀 **.hes Execution** - Run Humus Executable Scripts directly

## Building Humus

### Prerequisites

- GCC or compatible C compiler
- Make build system
- For full GUI support:
  - Windows: Win32 API (included with Windows SDK)
  - Linux: X11 or Wayland development libraries
  - macOS: Cocoa framework

### Build Instructions

#### Linux/macOS

```bash
cd build
make linux    # For Linux
make macos    # For macOS
```

#### Windows

```cmd
cd build
nmake windows    # Using Microsoft nmake
```

Or with MinGW:

```bash
cd build
make windows
```

### Build Outputs

- `humus` / `humus.exe` - Main executable
- `libhumus.a` / `humus.lib` - Static library for embedding
- `libhumus.so` / `humus.dll` - Shared library

## Usage

### Running .hes Files

```bash
# Run a .hes script
./humus myprogram.hes

# Or double-click .hes files (after file association setup)
```

### Command Line Options

```
Humus v0.1.0 - A Framework and Programming Language

Usage:
  humus [options] [file.hes]

Options:
  --help, -h     Show this help message
  --version, -v  Show version information
  --run, -r      Run a .hes file
  --demo         Run feature demos

Examples:
  humus program.hes           # Run a .hes script
  humus --demo                # Run feature demos
```

### File Association Setup

#### Windows

Create a registry file `associate_hes.reg`:

```reg
Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\.hes]
@="HumusScript"

[HKEY_CLASSES_ROOT\HumusScript\shell\open\command]
@="\"C:\\Path\\To\\humus.exe\" \"%1\""
```

#### Linux

Create a desktop entry `humus.desktop`:

```desktop
[Desktop Entry]
Type=Application
Name=Humus
Exec=/usr/local/bin/humus %f
MimeType=application/x-humus-script;
Icon=humus
```

Then run:
```bash
xdg-mime default humus.desktop application/x-humus-script
```

#### macOS

Use the `duti` command or create an Automator workflow to associate .hes files.

## Example .hes Programs

See the `examples/` directory for sample programs:

1. **hello.hes** - Hello World program
2. **graph_demo.hes** - Mathematical function plotting
3. **raytrace.hes** - Ray traced scene generation
4. **vector_art.hes** - Vector graphics creation
5. **chat_client.hes** - P2P chat example

## API Reference

### Core Functions

```c
// Initialize Humus
int hm_init(void);
void hm_shutdown(void);
```

### Window Management

```c
hm_window* hm_window_create(int width, int height, const char* title);
void hm_window_destroy(hm_window* win);
hm_bool hm_window_should_close(hm_window* win);
void hm_window_swap_buffers(hm_window* win);
void hm_window_poll_events(hm_window* win);
```

### UI Elements

```c
hm_button* hm_button_create(int x, int y, int width, int height, const char* label);
void hm_button_set_callback(hm_button* btn, void (*callback)(void*), void* user_data);
```

### Graphing

```c
hm_graph* hm_graph_create(const char* title);
void hm_graph_add_point(hm_graph* graph, hm_float64 x, hm_float64 y);
void hm_graph_plot_function(hm_graph* graph, hm_float64 (*func)(hm_float64), 
                            hm_float64 start, hm_float64 end, int samples);
```

### 3D & Ray Tracing

```c
hm_vec3 hm_3d_rotate_x(hm_vec3 v, hm_float64 angle);
hm_vec3 hm_3d_rotate_y(hm_vec3 v, hm_float64 angle);
hm_vec3 hm_3d_rotate_z(hm_vec3 v, hm_float64 angle);

hm_world* hm_world_create(void);
void hm_world_add_sphere(hm_world* world, hm_sphere* sphere);
hm_color hm_trace_ray(hm_world* world, hm_ray ray, int depth);
```

### Web Interaction

```c
hm_http_response* hm_http_get(const char* url);
hm_http_response* hm_http_post(const char* url, const char* data);
```

### P2P Chat

```c
hm_chatroom* hm_chatroom_create(void);
hm_bool hm_chatroom_connect(hm_chatroom* room, const char* address, int port);
hm_bool hm_chatroom_send(hm_chatroom* room, const char* message);
```

### Vector Images

```c
hm_vector_image* hm_vector_image_create(int width, int height);
void hm_vector_image_add_circle(hm_vector_image* img, hm_vec2 center, hm_float64 radius, ...);
void hm_vector_image_add_rect(hm_vector_image* img, hm_vec2 pos, hm_vec2 size, ...);
void hm_vector_image_add_line(hm_vector_image* img, hm_vec2 start, hm_vec2 end, ...);
hm_bool hm_vector_image_save_svg(hm_vector_image* img, const char* filename);
```

### VM Execution

```c
hm_vm* hm_vm_create(void);
hm_bool hm_vm_load_file(hm_vm* vm, const char* filename);
hm_bool hm_vm_run(hm_vm* vm);
```

## Embedding in Web Browsers

Humus can be compiled to WebAssembly for browser embedding:

```bash
# Compile to WebAssembly (requires Emscripten)
emcc src/*.c -I include -o humus.js -s WASM=1
```

Then use in HTML:

```html
<script src="humus.js"></script>
<script>
  Humus.init().then(() => {
    Humus.runFile('program.hes');
  });
</script>
```

## Architecture

```
humus/
├── include/          # Header files
│   └── humus_core.h  # Main API header
├── src/              # Source files
│   ├── humus_core.c  # Core implementation
│   └── main.c        # Entry point
├── examples/         # Example .hes programs
├── build/            # Build scripts and output
├── web/              # Web/embedding support
└── docs/             # Documentation
```

## The .hes Format

The `.hes` (Humus Executable Script) format is designed for quick execution:

```
.hes files contain:
- Bytecode header (magic number, version)
- Constant pool (strings, numbers)
- Function definitions
- Main entry point
- Optional metadata
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

MIT License - See LICENSE file for details

## Acknowledgments

Humus draws inspiration from:
- Java's "write once, run anywhere" philosophy
- C's performance and simplicity
- Modern scripting languages' ease of use

---

**Built with ❤️ in C**
