Natural Development Flow:

[DONE] Start with basic project setup and window management
    - MLX initialization
    - Window creation
    - Basic cleanup

[DONE] Implement map parsing and validation
    - File extension validation
    - Map file parsing
    - Color parsing
    - Texture path parsing
    - Player position detection

[DONE] Add basic player movement in 2D (for testing)
    - Key handlers (WASD + arrows)
    - Basic movement logic
    - Simple collision detection
    - 2D map display with:
        * Centered map view
        * White walls, dark gray floors
        * Red player marker
        * Green direction indicator

[NEXT] Implement basic raycasting without textures
    - Ray calculations
    - DDA algorithm
    - Wall height calculations
    - Basic 3D rendering

Add wall rendering with solid colors
    - Wall distance calculation
    - Perspective correction
    - Wall height based on distance

Implement texture loading and mapping
    - Load XPM textures
    - Calculate texture coordinates
    - Apply textures to walls

Add proper collision detection
    - Sliding along walls
    - Corner handling
    - Proper hitbox

Implement smooth movement and rotation
    - Frame-independent movement
    - Acceleration/deceleration
    - Smooth rotation

Add floor/ceiling colors
    - Floor ray calculations
    - Ceiling mirroring
    - Color gradient based on distance

Polish rendering and optimize performance
    - Optimize ray calculations
    - Add minimap
    - Performance improvements
