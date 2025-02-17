#ifndef STRUCT_H
#define STRUCT_H
#include "../libft/libft.h"
#include <../minilibx_opengl_20191021/mlx.h>
#include <stdio.h>
#include <math.h>

// Key codes for MacOS
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124

// Movement constants
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.1

typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

typedef struct s_texture
{
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_texture;

typedef struct s_player
{
    double  x;
    double  y;
    double  dir_x;     // Direction vector X (where player is looking)
    double  dir_y;     // Direction vector Y
    double  plane_x;   // Camera plane X (for FOV)
    double  plane_y;   // Camera plane Y
    int     move_w;    // Movement flags
    int     move_s;
    int     move_a;
    int     move_d;
    int     rot_left;
    int     rot_right;
} t_player;

typedef struct s_map
{
    char **map;
    int width;
    int height;
} t_map;

typedef struct s_ray
{
    double x;
    double y;
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
    t_texture   textures[4];  // Array to store loaded textures
} t_game;

// Function prototypes
int     validate_file_extension(const char *filename);
int     init_game(t_game *game);
int     parse_map_file(const char *filename, t_game *game);
void    cleanup_game(t_game *game);
void    print_game_state(t_game *game);
char    *get_next_line(int fd);

// New movement prototypes
int     key_press(int keycode, t_game *game);
int     key_release(int keycode, t_game *game);
int     game_loop(t_game *game);
void    move_player(t_game *game);

#endif