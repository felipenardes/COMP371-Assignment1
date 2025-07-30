# COMP 371 – Assignment 1
**Name:** Felipe Nardes  
**Student ID:** 40243669 
**Project Title:** Maze Explorer  

## Description
This 3D interactive application is a first-person maze explorer built using OpenGL. The player navigates through a textured maze with real-time lighting effects and collision detection. The objective is to reach the finish point marked as `F`.

## Controls
- `W` – Move forward  
- `S` – Move backward  
- `A` – Strafe left  
- `D` – Strafe right  
- `Mouse` – Look around (first-person camera)  
- `Shift` – Move faster  
- `ESC` – Exit the application

## Features
- Interactive first-person camera using keyboard and mouse  
- Collision detection with maze walls  
- Maze layout with start (`S`) and finish (`F`) positions  
- Dynamic lighting: moving, flickering torchlight that follows the player  
- Two distinct textured surfaces (walls and ground) not used in course tutorials  
- Colored and textured cube rendered using VAO and shaders  
- Finish detection triggers a success message and closes the game

## Technical Stack
- **OpenGL** (Core profile 3.3)  
- **GLFW** (Window/context/input)  
- **GLEW** (Extension loader)  
- **GLM** (Math library)  
- **stb_image** (Texture loading)

## How to Run
1. Build using your C++ compiler with all required libraries linked.
2. Place the textures used in the `Textures/` directory:
   - `ground_0044_color_1k.jpg` (ground)
   - `ground_0014_subsurface_1k.jpg` (walls)
3. Run the application from your terminal or IDE.

