# Table of Contents
* [Code Architecture Overview](#code-architecture-overview)
  * [Shaders](#shaders)
  * [Materials](#materials)
  * [Buffers & Layouts](#buffers--layouts)
  * [Textures](#textures)
  * [Shapes](#shapes)
  * [Renderer](#renderer)
  * [Camera](#camera)
  * [Debugger](#debugger)
* [OOP Concepts in Rendering Loop](#oop-concepts-in-rendering-loop)
  * [Inheritance & Polymorphism](#inheritance--polymorphism)
  * [Encapsulation](#encapsulation)
  * [Separation of Concerns](#separation-of-concerns)
* [How the Rendering Loop Works](#how-the-rendering-loop-works)
* [Demo Video](#demo-video)

## Code Architecture Overview

This project simulates a 3D solar system using OpenGL. Below is a breakdown of the key parts and their roles.

### Shaders
* **Color.shader:** Renders objects with a solid color. Uses `u_Color` uniform for coloring.
* **Texture.shader:**
  * Supports texture mapping and Phong lighting (ambient, diffuse, specular).
  * Vertex shader computes normals and positions for lighting.
  * Fragment shader blends textures with dynamic light calculations.

### Materials
* **Material (Base Class):**
  * Manages transformations (translation, scaling, rotation) and shader uniforms (`u_ModelMat`, `u_ViewMat`, `u_ProjectionMat`).
  * Abstract class with `setColorTexture()` as a pure virtual method.
* **ColorMaterial:**
  * Inherits from `Material`. Sets a uniform color via `u_Color`.
* **TextureMaterial:**
  * Inherits from `Material`. Binds textures and sets opacity via `u_TexOpacity`.

### Buffers & Layouts
* **VertexBuffer:** Stores vertex data (positions, UVs, normals).
* **ElementBuffer:** Stores indices for indexed rendering.
* **VertexArray:** Manages vertex attributes and buffer bindings.
* **VertexBufferLayout:** Defines vertex attribute formats (e.g., `position: vec3`, `UV: vec2`, `normal: vec3`).

### Textures
* Loads images (e.g., planet textures) using `stb_image`, binds to OpenGL texture slots.
* Textures are applied to `TextureMaterial` objects (e.g., sun, planets).

### Shapes
* **`getCube()`:** Generates vertices and indices for a cube (used for the skybox).
* **`getSphere()`:** Generates a UV-sphere mesh with normals (used for planets).

### Renderer
* **`draw()`:** Binds VAO/EBO/shader and issues `glDrawElements`.
* **`clear()`:** Clears color and depth buffers.

### Camera
* Manages view/projection matrices and FOV.
* Processes keyboard (WASD) and mouse input for movement/orbiting.
* Supports zoom via scroll.

### Debugger
* **Macros:** `glCall`, `ASSERT` wrap OpenGL calls to catch and log errors.
* **Functions:** `glClearError()`, `glLogCall()` for error checking.

## OOP Concepts in Rendering Loop

### Inheritance & Polymorphism
* **Base Class `Material`:** Defines common interfaces (`updateModelMat()`, `setColorTexture()`).
* **Derived Classes:** `ColorMaterial` and `TextureMaterial` override `setColorTexture()` to handle color/texture-specific logic.
* **Polymorphic Array:** All materials are stored as `Material*` pointers, enabling unified rendering:
  ```cpp
  Material* materials[] = { sunLight, sun, mercury, ... };
  for (Material* material : materials) {
    material->setColorTexture(); // Calls overridden method
    material->updateModelMat(deltaTime);
    renderer.draw(...);
  }
  ```

### Encapsulation
* Classes encapsulate OpenGL resources (e.g., `VertexBuffer`, `Texture`) with RAII:
    * Resources are initialized in constructors and released in destructors.
* Camera state (position, rotation) is managed internally, exposed via matrices.

### Separation of Concerns
* **Shaders** handle lighting/texturing logic.
* **Materials** manage object-specific properties (transformations, colors/textures).
* **Renderer/Camera** abstract low-level OpenGL details.

## How the Rendering Loop Works
1. **Initialization:**
    * Load shaders, textures, and create VAOs/VBOs for shapes.
    * Instantiate materials (e.g., `sun`, `earth`) with unique transforms and properties.
2. **Per-Frame Updates:**
    * Process input to update camera.
    * Compute view/projection matrices.
3. **Drawing:**
    * Render the skybox (`space` object) first.
    * Iterate over all `Material` objects, updating their transforms and binding resources.
    * Use `Renderer::draw()` to render each object with its VAO/EBO and shader.

This architecture ensures flexibility (e.g., adding new materials) and efficiency (shared rendering logic).
The use of OOP principles keeps the code modular and maintainable.

## Demo Video
[Solar System Simulation](video.mp4)
