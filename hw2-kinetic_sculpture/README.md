# Enhanced Kinetic Sculpture - 3D OpenGL Animation

A mesmerizing 3D kinetic sculpture created with OpenGL, featuring complex geometric patterns, dynamic lighting, and smooth animations using 3D transformation matrices.

## Screenshots

![Kinetic Sculpture Screenshot](assets/Screenshot%202025-09-29%20191150.png)

## Video Demo

https://github.com/user-attachments/assets/87aac4d9-5fdb-4065-a126-5a0bfe309f85


## Features

### 🎨 **Visual Design**
- **Central Spiral Tower**: Dynamic vertical structure with rotating elements
- **Orbiting Rings**: Multiple concentric rings with synchronized motion  
- **Floating Patterns**: Geometric formations creating depth and complexity
- **Gradient Background**: Dark cosmic atmosphere for enhanced visual appeal

### ⚡ **3D Transformation Matrices**
- **Translation Matrices**: Wave and spiral motion patterns
- **Rotation Matrices**: Multi-axis rotations (world and local space)
- **Scale Matrices**: Breathing/pulsing effects with dynamic sizing
- **Composite Transformations**: Complex combined matrix operations

### 💡 **Multiple Light Sources**
- **Directional Light**: Moonlight-style ambient illumination
- **6 Point Lights**: Each with unique colors and movement patterns:
  - Warm Orange Light
  - Cool Blue Light  
  - Purple Light
  - Green Light
  - Red Light
  - Cyan Light
- **Spotlight**: Camera-following illumination with adjustable cone

### 🔄 **Kinetic Animation**
- **Complex Motion Patterns**: Each cube follows unique mathematical trajectories
- **Breathing Effects**: Dynamic scaling with sine wave variations
- **Color Transitions**: Smooth RGB interpolation based on time and position
- **Synchronized Movement**: Coordinated light and object animations

## Technical Implementation

### Graphics Pipeline
- **OpenGL 3.3 Core Profile**
- **GLSL Shaders**: Custom vertex and fragment shaders
- **Multiple Lighting Models**: Phong shading with multiple light types
- **Texture Mapping**: Diffuse and specular texture support

### Mathematics
- **Parametric Equations**: Torus and spiral generation
- **Matrix Transformations**: GLM library for 3D math operations  
- **Trigonometric Functions**: Sin/cos waves for smooth animations
- **Vector Calculations**: Position, normal, and color computations

### Controls
- **WASD**: Camera movement (Forward/Back/Left/Right)
- **Mouse**: Look around (First-person camera)
- **Scroll Wheel**: Zoom in/out
- **ESC**: Exit application

## Building and Running

### Prerequisites
- Visual Studio 2022 (Windows)
- CMake 3.10+
- vcpkg package manager

### Dependencies (via vcpkg)
```bash
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows  
vcpkg install glm:x64-windows
vcpkg install stb:x64-windows
```

### Build Instructions
1. Clone the repository
2. Configure CMake with vcpkg toolchain:
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake
   ```
3. Build the project:
   ```bash
   cmake --build build --config Debug
   ```
4. Run the executable:
   ```bash
   ./build/Debug/hw2_kinetic_sculpture.exe
   ```

## Project Structure
```
hw2-kinetic_sculpture/
├── src/
│   ├── main.cpp           # Main application logic
│   ├── shader_m.h         # Shader management class
│   ├── camera.h           # Camera control system  
│   └── filesystem.h       # File path utilities
├── shaders/
│   ├── sculpture.vs       # Vertex shader
│   ├── sculpture.fs       # Fragment shader
│   ├── light_cube.vs      # Light cube vertex shader
│   └── light_cube.fs      # Light cube fragment shader
├── resources/textures/    # Texture assets
└── CMakeLists.txt         # Build configuration
```

## Asset Credits

### Textures
- **3D Pear Textures**: [3DPear004](https://ambientcg.com/view?id=3DPear004) from [ambientcg.com](https://ambientcg.com/)
  - `3DPear004_HQ-1K-PNG_Color.png` - Diffuse texture
  - `3DPear004_HQ-1K-PNG_Roughness.png` - Specular/roughness texture
  - License: CC0 (Public Domain)

*AmbientCG provides high-quality, royalty-free textures for 3D graphics projects.*

## Academic Context

This project was developed as a homework assignment focusing on:
- 3D transformation matrix operations
- Multiple lighting systems implementation  
- Interactive 3D graphics programming
- Real-time animation techniques

**Based on**: `src/2.lighting/6.multiple_lights/multiple_lights.cpp` from LearnOpenGL tutorials

## License

This project is created for educational purposes as part of a computer graphics course.

---

*Created with ❤️ using OpenGL, C++, and mathematical creativity*
