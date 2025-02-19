NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -L./minilibx_opengl_20191021 -lmlx -lm
LIBFT_FLAGS = -L libft -lft

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

# Source files following the structure.txt layout
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/init/init_game.c \
       $(SRC_DIR)/parser/file_reader.c \
       $(SRC_DIR)/parser/map_parser.c \
       $(SRC_DIR)/events/exit_handler.c \
       $(SRC_DIR)/events/key_handler.c \
       $(SRC_DIR)/movement/player_move.c \
       $(SRC_DIR)/render/render.c \
       $(SRC_DIR)/raycast/raycast.c \
       $(SRC_DIR)/utils/get_next_line.c \
       $(SRC_DIR)/utils/debug.c \
       $(SRC_DIR)/utils/error.c \
       $(SRC_DIR)/texture_loader.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Add libft directory
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Add MLX directory
MLX_DIR = minilibx_opengl_20191021
MLX = $(MLX_DIR)/libmlx.a

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(OBJS) $(MLX_FLAGS) -framework OpenGL -framework AppKit $(LIBFT_FLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MLX):
	@make -C $(MLX_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@make -C $(MLX_DIR) clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
