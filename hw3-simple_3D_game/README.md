# Simple 3D Dog Bone Collector Game

A delightful 3D adventure game where you control a cute dog exploring a scenic environment to collect bones while avoiding obstacles. Built with OpenGL and modern C++ for Assignment 3.

## Screenshots

<img width="1282" height="758" alt="image" src="https://github.com/user-attachments/assets/6deb88b8-3637-417a-b03b-4ee9b8cc92f1" />
<img width="1282" height="758" alt="image2" src="https://github.com/user-attachments/assets/0d199551-7d00-4e0b-a21d-c6b0daf34247" />


## Video Demo

https://github.com/user-attachments/assets/2d566e79-8b98-4750-8c8f-801f2eba8651


## Game Overview

🐕 Play as an adorable dog  
🦴 Collect 10 bones scattered around the environment  
🗑️ Navigate around obstacles like trash cans

## Features

### 🎮 **Gameplay Mechanics**
- **Third-Person Camera**: Smooth camera that follows behind the dog
- **Bone Collection**: Collect 10 rotating bones to complete the game
- **Collision System**: 
  - **Bone Collision**: Bones disappear when touched by the dog
  - **Obstacle Collision**: Dog cannot pass through trash cans and other objects
- **Rotating Animations**: 
  - Bones slowly rotate to attract attention
  - Dog can spin when holding the R key

### 🏗️ **3D Model Loading**
- **Player Model**: Animated dog character (`Dog.fbx`)
- **Scene Objects**: Detailed trash can obstacle (`Trash.fbx`)  
- **Collectible Items**: Bone models (`bone.fbx`)
- **Environment**: Realistic skybox with day lighting

### 📷 **Camera System**
- **Third-Person Follow**: Camera stays behind and above the dog
- **Smooth Movement**: Natural camera transitions during gameplay
- **Dynamic Positioning**: Camera adjusts based on dog's movement direction

### 💥 **Collision Detection**
- **Player-Item Collision**: AABB collision detection for bone collection
- **Player-Scene Collision**: Prevents dog from walking through solid objects
- **Precise Boundaries**: Collision boxes accurately match model dimensions

## Controls

- **W/A/S/D**: Move the dog (Forward/Left/Back/Right)
- **R (Hold)**: Make the dog spin in place
- **ESC**: Exit the game

## Technical Implementation

### Graphics Pipeline
- **OpenGL 3.3 Core Profile**
- **Modern Shader Pipeline**: Custom vertex and fragment shaders
- **Model Loading**: Assimp library for 3D model importing
- **Texture Mapping**: Support for diffuse and material textures

### Game Architecture
- **Entity-Component System**: Modular player, camera, and collision classes
- **Real-time Physics**: Frame-rate independent movement and collision
- **Resource Management**: Efficient loading and rendering of 3D assets

### Prerequisites
- **CMake** 3.16 or higher
- **Visual Studio 2022** (Windows)
- **Internet connection** for dependency fetching

### Dependencies (Auto-fetched via CMake)
- **GLFW**: Window management and input
- **GLAD**: OpenGL function loading
- **GLM**: Mathematics library for 3D operations
- **Assimp**: 3D model loading
- **stb_image**: Texture loading


## Project Structure
```
hw3-simple_3D_game/
├── src/
│   ├── main.cpp           # Main game loop and rendering
│   ├── Player.cpp         # Player movement and controls
│   ├── Camera.cpp         # Third-person camera system
│   ├── Collision.cpp      # AABB collision detection
│   ├── Model.cpp          # 3D model loading and rendering
│   ├── Mesh.cpp           # Mesh data management
│   └── Shader.cpp         # Shader compilation and management
├── include/
│   ├── Player.h           # Player class definitions
│   ├── Camera.h           # Camera class definitions
│   ├── Collision.h        # Collision system headers
│   ├── Model.h            # Model loading headers
│   ├── Mesh.h             # Mesh structure definitions
│   └── Shader.h           # Shader management headers
├── shaders/
│   ├── model.vert         # Vertex shader for 3D models
│   ├── model.frag         # Fragment shader with lighting
│   ├── skybox.vert        # Skybox vertex shader
│   └── skybox.frag        # Skybox fragment shader
├── assets/
│   ├── models/            # 3D model files
│   ├── textures/          # Texture files
│   └── cubemap/           # Skybox texture faces
└── CMakeLists.txt         # Build configuration
```

## Asset Credits

### 3D Models
- **Dog Model**: [Dog Puppy](https://sketchfab.com/3d-models/dog-eb3fbf324cce4ecc9f64d99a9c0c23ba) by Sketchfab
- **Bone Model**: [Bone](https://sketchfab.com/3d-models/bone-3847804e80d243809b6d3fc1bf4ad965) by Sketchfab  
- **Trash Can**: [Trash Metal](https://sketchfab.com/3d-models/trash-metal-4mb-6c3491c2128d40ae9621cf382b9ab045) by Sketchfab

### Environment
- **Skybox HDRI**: [Day Environment HDRI048](https://acg-download.struffelproductions.com/file/ambientCG-Web/download/DayEnvironmentHDRI048_BydQDoxF/DayEnvironmentHDRI048_4K-TONEMAPPED.jpg) from AmbientCG
- **Cubemap Conversion**: Panorama converted to cubemap using [Panorama to Cubemap Tool](https://jaxry.github.io/panorama-to-cubemap/)
- **File Naming**: After conversion, rename the cubemap faces as follows:
  - `px` → `right.png`
  - `nx` → `left.png`
  - `py` → `top.png`
  - `ny` → `bottom.png`
  - `pz` → `front.png`
  - `nz` → `back.png`

*All assets are used in accordance with their respective licenses for educational purposes.*

## Assignment Requirements Fulfilled

✅ **Models loading from file**: Player (dog), scene objects (trash can), items (bones)  
✅ **Player control**: WASD movement with smooth third-person controls  
✅ **Camera follows player**: Dynamic third-person camera system  
✅ **Collision detection**: Player-item (bone collection) and player-scene (obstacle avoidance)  

## Academic Context

This project was developed as **Assignment 3** for a 3D Graphics Programming course, demonstrating:
- 3D model loading and rendering techniques
- Real-time collision detection algorithms  
- Camera control systems for games
- Interactive 3D game development principles

## License

This project is created for educational purposes as part of a computer graphics assignment.

---

*Created with 🐕 love using OpenGL, C++, and 3D game development techniques*
