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

[IN PROGRESS] Implement basic raycasting without textures
    [DONE] Ray calculations
        * Ray structure defined ✓
        * Direction calculations implemented ✓
        * Basic ray visualization ✓
    [NEXT] DDA algorithm
        * Calculate step and side distances
        * Implement wall hit detection
        * Calculate perpendicular wall distance
    [TODO] Wall height calculations
    [TODO] Basic 3D rendering

[TODO] Add wall rendering with solid colors
    - Wall distance calculation
    - Perspective correction
    - Wall height based on distance

[TODO] Implement texture loading and mapping
    - Load XPM textures
    - Calculate texture coordinates
    - Apply textures to walls

[TODO] Add proper collision detection
    - Sliding along walls
    - Corner handling
    - Proper hitbox

[TODO] Implement smooth movement and rotation
    - Frame-independent movement
    - Acceleration/deceleration
    - Smooth rotation

[TODO] Add floor/ceiling colors
    - Floor ray calculations
    - Ceiling mirroring
    - Color gradient based on distance

[TODO] Polish rendering and optimize performance
    - Optimize ray calculations
    - Add minimap
    - Performance improvements

Current Status:
- Basic 2D rendering and movement system complete
- Player can move with both WASD and arrow keys
- Basic collision detection implemented
- FOV visualization with ray casting (visual only)
- Clean program exit with ESC key or window close
- Next step: Implement DDA algorithm for ray-wall collision
