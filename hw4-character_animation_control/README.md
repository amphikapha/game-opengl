# Character Animation Control - Assignment 4

## Overview
A playable 3D character animation system where users can control both the character movement and animations in real-time using keyboard inputs. The character model is sourced from **Mixamo** and features smooth skeletal animation with multiple animation states.

## 📦 Character Model Source

**Model**: Doozy Character  
**Source**: [Mixamo - Doozy Character](https://www.mixamo.com/#/?page=1&query=doozy&type=Character)  
**Animation Provider**: Adobe Mixamo  

The character model and animations have been downloaded from Mixamo and integrated into this project using Assimp for skeletal animation loading.

### Animations Included
- **Happy Idle**: Neutral standing pose
- **Walking**: Looped walking cycle
- **Gangnam Style**: Entertaining dance animation

## 🎮 **Animation Control System**
- **Walking Animation**: Character walks when moving with WASD keys
- **Dancing Animation**: Character performs Gangnam Style dance when pressing E
- **Real-time Switching**: Smooth transitions between different animations

### 🕹️ **Character Controls**
- **W**: Move forward while playing walk animation
- **S**: Move backward while playing walk animation  
- **A**: Turn left while playing walk animation
- **D**: Turn right while playing walk animation
- **E**: Play dance animation (Gangnam Style)

### 📷 **Camera Controls**
- **Arrow Keys**: Move camera for better viewing angles
- **Mouse**: Look around (first-person camera view)
- **Scroll Wheel**: Zoom in/out

## Project Structure
```
hw4-character_animation_control/
├── src/
│   ├── main.cpp           # Main application with character control logic
│   ├── animation.h        # Animation loading and playback
│   ├── animator.h         # Animation state management
│   ├── model_animation.h  # 3D model with animation support
│   └── [other headers]    # Supporting animation classes
├── shaders/
│   ├── anim_model.vs      # Vertex shader with bone transformations
│   └── anim_model.fs      # Fragment shader for animated models
├── resources/
│   └── objects/doozy/     # Character model and animations
│       ├── Walking.dae    # Walking animation
│       └── Gangnam Style.dae # Dance animation
└── CMakeLists.txt         # Build configuration
```

---

## 🎬 **Gameplay Video**

https://github.com/user-attachments/assets/e8413c84-329d-4dd6-8577-ab8d3806230d

