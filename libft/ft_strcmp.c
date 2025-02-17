/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athill <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 08:49:53 by athill            #+#    #+#             */
/*   Updated: 2024/04/22 10:55:38 by athill           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(char const *s1, char const *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_streq(char const *s1, char const *s2)
{
	return (ft_strcmp(s1, s2) == 0);
}
