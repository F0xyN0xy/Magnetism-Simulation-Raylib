# Magnetic Field Simulator — C++ / Raylib Edition

A refined, GPU-accelerated desktop application for exploring magnetic fields interactively. Built with **C++17** and **Raylib**.

> This is the successor to the Python/Pygame prototype. It features a proper application UI (panels, sliders, buttons), GPU-batched rendering, and smooth 60 FPS performance even with many magnets and thousands of particles.

---

## Screenshots

*Main view with field lines, a selected magnet, and the right-side control panel.*

---

## What You Get

- **Interactive bar magnets** — drag to move, right-click to rotate, scroll to change strength
- **Real-time field lines** — traced from north to south poles, updating every frame
- **Vector arrow grid** — toggleable B-field direction arrows
- **Iron filings** — up to 3,000 particles that align with and drift along the field
- **Compass grid** — needles that track the local field direction
- **Application-grade UI** — toolbar, collapsible control panel, status bar, help overlay
- **Physics sliders** — adjust damping, strength multiplier, and simulation speed in real time
- **Scene presets** — reset to default arrangements instantly
- **Resizable window** — all UI elements reposition and clamp automatically

---

## Performance vs. Python

| Metric | Python + Pygame | C++ + Raylib |
|--------|----------------|--------------|
| Renderer | CPU (software) | GPU (OpenGL) |
| 1,500 iron filings | ~25–35 FPS | 60+ FPS |
| 10 magnets + all overlays | ~15 FPS | 60+ FPS |
| Field lines (28 × 300 segments) | Heavy CPU draw | GPU batched |
| UI | Keyboard-only | Full GUI panels |

---

## System Requirements

| Requirement | Minimum | Recommended |
|-------------|---------|-------------|
| **OS** | Windows 10, macOS 11, Linux (Ubuntu 20.04+) | Latest stable |
| **Compiler** | GCC 9+, Clang 12+, MSVC 2019+ | GCC 12+, Clang 15+ |
| **OpenGL** | 3.3 | 4.5+ |
| **RAM** | 2 GB | 4 GB |
| **Display** | 1280×800 | 1920×1080 |

---

## Installing Raylib

You need **Raylib 5.0+** installed on your system before compiling.

### Linux (Ubuntu / Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev -y

# Build Raylib from source (recommended for latest version)
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
make -j$(nproc)
sudo make install
sudo ldconfig

# Verify
pkg-config --exists raylib && echo "Raylib installed"
```

### Linux (Fedora)

```bash
sudo dnf install alsa-lib-devel libX11-devel libXrandr-devel libXi-devel mesa-libGL-devel libXcursor-devel libXinerama-devel -y

# Then build from source as shown above
```

### macOS

```bash
# Using Homebrew (may not be the absolute latest)
brew install raylib

# Or build from source for the latest:
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
make -j$(sysctl -n hw.ncpu)
sudo make install
```

### Windows (MinGW)

1. Download the **Raylib Windows Installer** from [raysan5.com/raylib](https://www.raylib.com/).
2. Run the installer and note the install path (e.g., `C:\raylib`).
3. The installer sets up MinGW and adds it to your PATH.
4. Verify in Command Prompt:
   ```cmd
   gcc --version
   ```

### Windows (Visual Studio)

1. Download the **Raylib VS2022 project template** from the Raylib website.
2. Or use `vcpkg`:
   ```cmd
   vcpkg install raylib
   ```

---

## Building the Project

### Step 1: Clone or Download

```bash
git clone <repo-url>
cd magnetism_sim_cpp
```

### Step 2: Compile

**Linux / macOS:**
```bash
make
```

**Windows (MinGW):**
```cmd
mingw32-make
```

**Windows (MSVC):**
```cmd
cl /EHsc /O2 /W4 main.cpp magnet.cpp physics.cpp field.cpp particles.cpp compass.cpp renderer.cpp ui.cpp /link raylib.lib opengl32.lib gdi32.lib winmm.lib
```

**Manual compile (any platform):**
```bash
g++ -std=c++17 -O2 -o magnetism main.cpp magnet.cpp physics.cpp field.cpp particles.cpp compass.cpp renderer.cpp ui.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
*(Adjust `-l` flags for your OS — see Makefile for platform-specific flags.)*

### Step 3: Run

```bash
./magnetism
```

On Windows:
```cmd
magnetism.exe
```

---

## Controls

### Mouse

| Action | Result |
|--------|--------|
| **Left Click** on magnet | Select it |
| **Left Click + Drag** | Move magnet |
| **Right Click** on magnet | Rotate toward cursor |
| **Mouse Wheel** | Adjust selected magnet strength |

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| **Space** | Pause / Resume |
| **R** | Reset scene |
| **N** | Add magnet |
| **Delete / Backspace** | Remove selected |
| **F** | Toggle vector arrows |
| **I** | Toggle iron filings |
| **C** | Toggle compasses |
| **H** | Toggle help overlay |
| **Esc** | Quit |

### UI Panel (Right Side)

- **Checkboxes** — Toggle field lines, arrows, filings, compasses
- **Strength slider** — Global magnet strength preset
- **Damping slider** — How quickly magnets slow down
- **Speed slider** — Simulation time scale (slow-mo to fast-forward)
- **Particle count slider** — Iron filing density
- **Apply button** — Copy global strength to selected magnet

---

## Project Architecture

```
magnetism_sim_cpp/
├── main.cpp          # Entry point, game loop, input handling
├── magnet.hpp/cpp    # Magnet entity (position, rotation, poles, rendering)
├── physics.hpp/cpp   # Pole-pole force & torque integration
├── field.hpp/cpp     # B-field computation, field-line tracing, arrow grid
├── particles.hpp/cpp # Iron filings (positions, angles, drift)
├── compass.hpp/cpp   # Compass needle grid
├── renderer.hpp/cpp  # All drawing code (GPU-accelerated via Raylib)
├── ui.hpp/cpp        # Raygui panels, sliders, buttons, status bar
├── constants.hpp     # Colors, dimensions, physics defaults
├── Makefile          # Cross-platform build
└── README.md         # This file
```

### Design Decisions

- **GPU rendering:** All lines, polygons, and particles are drawn via Raylib's OpenGL backend — thousands of draw calls per frame are batched automatically.
- **No numpy needed:** C++ loops are fast enough that simple per-particle field queries run comfortably at 60 FPS without vectorization libraries.
- **Immediate-mode GUI:** Raygui provides buttons, sliders, and checkboxes with minimal code — no external UI framework needed.
- **Monopole approximation:** Each magnet = two opposite point charges. Simple, intuitive, and produces believable dipole behavior.

---

## Customizing

Edit `constants.hpp` to tweak defaults:

```cpp
constexpr int   FIELD_LINES_PER_MAGNET = 20;   // More lines
constexpr int   FIELD_MAX_STEPS        = 400;  // Longer traces
constexpr int   MAX_FILINGS            = 3000; // More particles
constexpr float MAX_FORCE              = 8000; // Stronger interactions
```

Rebuild with `make` after changing constants.

---

## Troubleshooting

### "raylib.h: No such file or directory"

Raylib is not installed or not in your include path.

**Fix:**
```bash
# Linux: make sure you ran `sudo make install` after building raylib
# macOS: same, or use `brew install raylib`
# Windows: add `-I C:\raylib\include` to your compile flags
```

### "undefined reference to `InitWindow`"

The linker cannot find the Raylib library.

**Fix:**
```bash
# Linux: make sure `sudo ldconfig` was run after install
# Or specify the path explicitly:
g++ ... -L/usr/local/lib -lraylib ...
```

### Black screen on launch

Your GPU driver may not support OpenGL 3.3.

**Fix:** Update your graphics drivers. Raylib falls back to OpenGL 2.1 on older hardware, but some features may not work.

### Low FPS on old hardware

Reduce visual density in the right panel (lower particle count, disable field lines) or edit `constants.hpp` to reduce `FIELD_LINES_PER_MAGNET` and `MAX_FILINGS`.

### Window is too small / UI overlaps

The default window is 1440×900. Resize the window by dragging the corner — all UI panels auto-reposition. Minimum recommended: 1024×768.

---

## License

Provided as an educational resource. Modify and share freely.

---

## Credits

Built with **C++17**, **Raylib 5.0+**, and **raygui**. No external game engine.
