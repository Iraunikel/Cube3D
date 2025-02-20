#ifndef STRUCT_H
#define STRUCT_H

#include "../libft/libft.h"
#include "../minilibx_opengl_20191021/mlx.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

// Window dimensions
# define WINDOW_WIDTH 1000
# define WINDOW_HEIGHT 1000

// Key codes for MacOS
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_UP 126
# define KEY_DOWN 125

// Movement constants
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.1
# define MAX_SPEED 0.15
# define ACCELERATION 0.01
# define DECELERATION 0.008
# define MAX_ROT_SPEED 0.12

// Texture indices
# define TEX_NORTH 0
# define TEX_SOUTH 1
# define TEX_EAST  2
# define TEX_WEST  3
# define TEX_FLOOR 4
# define TEX_CEIL  5
# define NUM_TEXTURES 6

// Forward declarations of structs
typedef struct s_game t_game;
typedef struct s_texture t_texture;
typedef struct s_player t_player;
typedef struct s_map t_map;
typedef struct s_ray t_ray;
typedef struct s_color t_color;

// Error handling
void    error_exit(const char *message);

typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

typedef struct s_texture
{
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
} t_texture;

typedef struct s_player
{
    double  x;
    double  y;
    double  dir_x;     // Direction vector X (where player is looking)
    double  dir_y;     // Direction vector Y
    double  plane_x;   // Camera plane X (for FOV)
    double  plane_y;   // Camera plane Y
    double  hitbox_radius;  // Player's hitbox radius
    double  wall_buffer;    // Minimum distance from walls
    int     move_w;    // Forward
    int     move_s;    // Backward
    int     move_a;    // Strafe left
    int     move_d;    // Strafe right
    int     rot_left;  // Turn left (arrow)
    int     rot_right; // Turn right (arrow)
} t_player;

typedef struct s_map
{
    char    **map;
    int     width;
    int     height;
} t_map;

typedef struct s_ray
{
    double  pos_x;      // Ray position X
    double  pos_y;      // Ray position Y
    double  dir_x;      // Ray direction X
    double  dir_y;      // Ray direction Y
    double  delta_dist_x;   // Length of ray from one x-side to next
    double  delta_dist_y;   // Length of ray from one y-side to next
    int     map_x;      // Which box of the map we're in
    int     map_y;
    double  side_dist_x;    // Length of ray from current pos to next x-side
    double  side_dist_y;    // Length of ray from current pos to next y-side
    int     step_x;     // What direction to step in x direction (+1 or -1)
    int     step_y;     // What direction to step in y direction (+1 or -1)
    int     hit;        // Was there a wall hit?
    int     side;       // Was a NS or a EW wall hit?
    double  perp_wall_dist; // Perpendicular wall distance
    double  wall_x;     // Where exactly the wall was hit
} t_ray;

typedef struct s_game
{
    void        *mlx;
    void        *win;
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
    t_player    player;
    t_map       map;
    t_color     ceiling_color;
    t_color     floor_color;
    char        *north_texture;
    char        *south_texture;
    char        *west_texture;
    char        *east_texture;
    char        *floor_texture;    // New: path to floor texture
    char        *ceiling_texture;  // New: path to ceiling texture
    t_texture   textures[NUM_TEXTURES];  // Expanded array to store all textures
    int         use_texture_floor;   // Flag to use texture instead of color for floor
    int         use_texture_ceiling; // Flag to use texture instead of color for ceiling
} t_game;

typedef struct s_floor_calc
{
    double  player_x;
    double  player_y;
    double  row_distance;
    double  floor_step_x;
    double  floor_step_y;
    double  floor_x;
    double  floor_y;
} t_floor_calc;

// Function prototypes
int     validate_file_extension(const char *filename);
int     init_game(t_game *game);
int     parse_map_file(const char *filename, t_game *game);
void    cleanup_game(t_game *game);
void    print_game_state(t_game *game);
char    *get_next_line(int fd);
void    load_textures(t_game *game);
unsigned int get_texture_color(t_texture *tex, int x, int y);

// Movement prototypes
int     key_press(int keycode, t_game *game);
int     key_release(int keycode, t_game *game);
int     game_loop(t_game *game);
void    move_player(t_game *game);

// Raycasting functions
void    calculate_ray_dir(t_ray *ray, t_player *player, double camera_x);
void    cast_rays(t_game *game);
void    draw_ray_2d(t_game *game, t_ray *ray);
void    draw_3d_view(t_game *game, t_ray *ray, int x);
void    init_dda(t_ray *ray);
void    perform_dda(t_ray *ray, t_game *game);
void    draw_textured_wall(t_game *game, t_ray *ray, int x);


#endif