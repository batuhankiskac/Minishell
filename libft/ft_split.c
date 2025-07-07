/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 23:24:51 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
			{
				s++;
			}
		}
	}
	return (count);
}

static int	ft_word_len(char const *s, char c)
{
	int	len;

	len = 0;
	while (*s && *s != c)
	{
		len++;
		s++;
	}
	return (len);
}

static void	*freeall(char **res, int i)
{
	while (i > 0)
		free(res[i--]);
	free(res);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;
	int		len;

	i = 0;
	res = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			len = ft_word_len(s, c);
			res[i] = (char *)malloc((len + 1) * sizeof(char));
			if (res[i] == NULL)
				return (freeall(res, i));
			ft_strlcpy(res[i++], s, len + 1);
			s += len;
		}
	}
	res[i] = NULL;
	return (res);
}
