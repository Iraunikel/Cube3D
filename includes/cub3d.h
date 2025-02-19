typedef struct s_texture {
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
} t_texture;

typedef struct s_textures {
    t_texture north;
    t_texture south;
    t_texture east;
    t_texture west;
} t_textures;

typedef struct s_ray {
    double  dir_x;
    double  dir_y;
    int     map_x;
    int     map_y;
    double  side_dist_x;
    double  side_dist_y;
    double  delta_dist_x;
    double  delta_dist_y;
    double  perp_wall_dist;
    int     step_x;
    int     step_y;
    int     hit;
    int     side;
    double  wall_x;
} t_ray;

typedef struct s_game {
    void        *mlx;
    void        *win;
    t_img       img;
    t_map       map;
    t_player    player;
    t_textures  textures;
} t_game;

// Function prototypes
void            load_textures(t_game *game);
void            draw_textured_wall(t_game *game, t_ray *ray, int x);
unsigned int    get_texture_color(t_texture *tex, int x, int y); 