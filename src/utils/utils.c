/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:33:35 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;
	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	ft_free_all(char **arr)
{
	int	i;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

