#include <stddef.h>
#include <stdlib.h> 
#include "libft.h"


static void	ft_split_helper(const char *s, char c, char **array)
{
	size_t	i;
	size_t	str_count;

	i = 0;
	str_count = 1;
	while (s[i])
	{
		if (s[i] == c)
		{
			str_count = 2;
			array[0] = ft_substr_end(s, 0, i);
			array[1] = ft_substr_end(s, i + 1, ft_strlen(s));
			break ;
		}
		i++;
	}
	if (str_count == 1)
		array[0] = ft_substr_end(s, 0, ft_strlen(s));
	array[str_count] = 0;
}

char	**ft_split_env(const char *s, char c)
{
	size_t	i;
	size_t	str_count;
	char	**array;

	i = 0;
	str_count = 1;
	while (s[i])
	{
		if (s[i] == c)
		{
			str_count = 2;
			break ;
		}
		i++;
	}
	array = malloc((str_count + 1) * sizeof(char *));
	if (array == 0)
		return (0);
	ft_split_helper(s, c, array);
	return (array);
}