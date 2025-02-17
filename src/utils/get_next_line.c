#include "../includes/cube3d.h"

#define BUFFER_SIZE 4096

static char	*resize_buffer(char *line, size_t *size, size_t i)
{
	char	*tmp;
	size_t	j;

	if (i < SIZE_MAX / 2)
		*size *= 2;
	else
		*size = SIZE_MAX;
	tmp = malloc(*size);
	if (!tmp)
	{
		free(line);
		return (NULL);
	}
	j = 0;
	while (j < i)
	{
		tmp[j] = line[j];
		j++;
	}
	free(line);
	return (tmp);
}

static char	*process_line(char *line, char *buf, size_t *idx, size_t *i,
		size_t *size, size_t *bytes_read)
{
	(void)bytes_read;
    while (*i < *size - 1)
	{
		line[(*i)++] = buf[(*idx)++];
		if (line[*i - 1] == '\n')
			return (line);
	}
	line = resize_buffer(line, size, *i);
	return (line);
}

static char	*read_and_process(int fd, char *line, char *buf, size_t size)
{
	static size_t	idx;
	static size_t	bytes_read;
	size_t			i;

	i = 0;
	while (1)
	{
		if (idx >= bytes_read)
		{
			bytes_read = read(fd, buf, BUFFER_SIZE);
			if (bytes_read <= 0)
				break ;
			idx = 0;
		}
		line = process_line(line, buf, &idx, &i, &size, &bytes_read);
		if (!line || (i > 0 && line[i - 1] == '\n'))
			break ;
	}
	if (i == 0 || !line)
	{
		free(line);
		return (NULL);
	}
	line[i] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE];
	char		*line;
	size_t		size;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	size = BUFFER_SIZE + 1;
	line = malloc(size);
	if (!line)
		return (NULL);
	return (read_and_process(fd, line, buf, size));
}
