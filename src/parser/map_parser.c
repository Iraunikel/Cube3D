/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:39:08 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 21:51:16 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cube3d.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <stdlib.h>

static int parse_texture_path(char *line, char **texture_path)
{
    char *trimmed;

    trimmed = ft_strtrim(line + 2, " \t\n\r\v\f");
    if (!trimmed)
        return (0);
    *texture_path = trimmed;
    return (1);
}

static void free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

static int parse_color(char *line, t_color *color)
{
    char    **rgb;
    int     values[3];
    int     i;

    rgb = ft_split(line + 1, ',');
    if (!rgb)
        return (0);
    
    i = 0;
    while (rgb[i] && i < 3)
    {
        values[i] = ft_atoi(rgb[i]);
        if (values[i] < 0 || values[i] > 255)
        {
            free_array(rgb);
            return (0);
        }
        i++;
    }
    
    if (i != 3)
    {
        free_array(rgb);
        return (0);
    }
    
    color->r = values[0];
    color->g = values[1];
    color->b = values[2];

    printf("Debug - Parsed color: R(%d) G(%d) B(%d)\n", color->r, color->g, color->b);

    free_array(rgb);
    return (1);
}

static int allocate_map(t_map *map, char **lines, int start_line)
{
    int i;
    int max_width;

    // Count number of lines for map height
    i = start_line;
    max_width = 0;
    while (lines[i])
    {
        if (ft_strlen(lines[i]) > (size_t)max_width)
            max_width = ft_strlen(lines[i]);
        i++;
    }
    
    map->height = i - start_line;
    map->width = max_width;
    
    // Allocate map array
    map->map = (char **)malloc(sizeof(char *) * (map->height + 1));
    if (!map->map)
        return (0);
    map->map[map->height] = NULL;
    
    return (1);
}

static void find_player_position(t_game *game, int i)
{
    int j;

    j = 0;
    while (game->map.map[i][j])
    {
        if (ft_strchr("NSEW", game->map.map[i][j]))
        {
            game->player.x = j + 0.5;
            game->player.y = i + 0.5;
            break;
        }
        j++;
    }
}

static int copy_map_line(t_game *game, char **lines, int i, int start_line)
{
    game->map.map[i] = ft_strdup(lines[start_line + i]);
    if (!game->map.map[i])
    {
        while (--i >= 0)
            free(game->map.map[i]);
        free(game->map.map);
        return (0);
    }
    find_player_position(game, i);
    return (1);
}

static int parse_map_content(t_game *game, char **lines, int start_line)
{
    int i;
    
    if (!allocate_map(&game->map, lines, start_line))
        return (0);
    
    i = 0;
    while (i < game->map.height)
    {
        if (!copy_map_line(game, lines, i, start_line))
            return (0);
        i++;
    }
    return (1);
}

static char **read_file_contents(int fd)
{
    char    *line;
    char    **contents;
    int     line_count;
    int     i;

    // First count the lines
    line_count = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        free(line);
        line_count++;
    }
    
    // Allocate array for lines
    contents = (char **)malloc(sizeof(char *) * (line_count + 1));
    if (!contents)
        return (NULL);
    contents[line_count] = NULL;

    // Reset file position and read lines
    lseek(fd, 0, SEEK_SET);
    i = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        contents[i] = line;
        i++;
    }

    return (contents);
}

static int parse_floor_ceiling(char *line, t_game *game)
{
    // Check if it's a texture path
    if (line[1] == 'T')
    {
        if (line[0] == 'F')
        {
            game->use_texture_floor = 1;
            return parse_texture_path(line + 1, &game->floor_texture);
        }
        else if (line[0] == 'C')
        {
            game->use_texture_ceiling = 1;
            return parse_texture_path(line + 1, &game->ceiling_texture);
        }
        return (0);
    }
    // Otherwise, parse as color
    else if (line[1] == ' ')
    {
        if (line[0] == 'F')
        {
            game->use_texture_floor = 0;
            return parse_color(line, &game->floor_color);
        }
        else if (line[0] == 'C')
        {
            game->use_texture_ceiling = 0;
            return parse_color(line, &game->ceiling_color);
        }
    }
    return (0);
}

static int parse_texture_line(char *line, t_game *game)
{
    if (ft_strncmp(line, "NO ", 3) == 0)
        return parse_texture_path(line, &game->north_texture);
    if (ft_strncmp(line, "SO ", 3) == 0)
        return parse_texture_path(line, &game->south_texture);
    if (ft_strncmp(line, "WE ", 3) == 0)
        return parse_texture_path(line, &game->west_texture);
    if (ft_strncmp(line, "EA ", 3) == 0)
        return parse_texture_path(line, &game->east_texture);
    return (1);
}

static int process_map_line(char *line, int *map_start, int i)
{
    if (ft_strchr("01NSEW ", line[0]))
    {
        if (*map_start == -1)
            *map_start = i;
    }
    return (1);
}

static int parse_line_content(char *line, t_game *game, int *map_start, int i)
{
    if (line[0] == '\0')
        return (1);
    
    if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
        ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
        return parse_texture_line(line, game);
    
    if (line[0] == 'F' || line[0] == 'C')
        return parse_floor_ceiling(line, game);
    
    return process_map_line(line, map_start, i);
}

int parse_map_file(const char *filename, t_game *game)
{
    int     fd;
    char    **file_contents;
    int     i;
    int     map_start;
    char    *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (0);

    file_contents = read_file_contents(fd);
    close(fd);
    if (!file_contents)
        return (0);

    i = 0;
    map_start = -1;
    while (file_contents[i])
    {
        line = ft_strtrim(file_contents[i], " \t\n\r\v\f");
        if (!line || !parse_line_content(line, game, &map_start, i))
        {
            free(line);
            free_array(file_contents);
            return (0);
        }
        free(line);
        i++;
    }

    if (map_start == -1 || !parse_map_content(game, file_contents, map_start))
    {
        free_array(file_contents);
        return (0);
    }

    free_array(file_contents);
    return (1);
} 