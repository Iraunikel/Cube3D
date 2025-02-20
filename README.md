Natural Development Flow:

[DONE] Start with basic project setup and window management
    - MLX initialization ✓
    - Window creation ✓
    - Basic cleanup ✓
    - Error handling ✓

[DONE] Implement map parsing and validation
    - File extension validation ✓
    - Map file parsing ✓
    - Color parsing ✓
    - Texture path parsing ✓
    - Player position detection ✓
    - Basic error handling ✓

[DONE] Add basic player movement in 2D (for testing)
    - Key handlers (WASD + arrows) ✓
    - Basic movement logic ✓
    - Simple collision detection ✓
    - 2D map display with:
        * Centered map view ✓
        * White walls, dark gray floors ✓
        * Red player marker ✓
        * Green direction indicator ✓
        * Blue FOV rays ✓

[DONE] Implement basic raycasting without textures
    [DONE] Ray calculations
        * Ray structure defined ✓
        * Direction calculations implemented ✓
        * Basic ray visualization ✓
    [DONE] DDA algorithm
        * Calculate step and side distances ✓
        * Implement wall hit detection ✓
        * Calculate perpendicular wall distance ✓
        * Proper ray visualization with wall stopping ✓
    [DONE] Wall height calculations
        * Calculate wall height based on distance ✓
        * Implement vertical line drawing ✓
        * Add basic 3D perspective ✓
    [DONE] Basic 3D rendering
        * Split screen implementation (2D/3D) ✓
        * Floor/ceiling colors ✓
        * Wall shading based on orientation ✓
        * Performance optimization ✓
        * Fix visual artifacts ✓

[DONE] Implement texture loading and mapping
    - Load XPM textures ✓
    - Calculate texture coordinates ✓
    - Apply textures to walls ✓
    - Proper texture scaling and alignment ✓

[DONE] Add proper collision detection
    - Sliding along walls ✓
    - Corner handling ✓
    - Proper hitbox ✓
    - Wall distance buffer ✓

[TODO] Implement smooth movement and rotation
    - Frame-independent movement
    - Acceleration/deceleration
    - Smooth rotation
    - Mouse look support (optional)

[TODO] Add advanced rendering features
    - Floor and ceiling textures
    - Distance fog effect
    - Dynamic lighting (optional)
    - Sprite rendering (optional)

[TODO] Polish and optimize
    - Optimize ray calculations
    - Add configurable minimap
    - Performance improvements
    - Code cleanup and documentation
    - Memory management optimization
