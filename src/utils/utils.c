/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/01 14:47:32 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees a null-terminated array of strings.
 *
 * This function iterates through the array of strings, freeing each
 * string, and then frees the array itself.
 *
 * @param arr A null-terminated array of strings to free.
 */
void	ft_free_all(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

/**
 * @brief Frees a linked list of redirection nodes.
 *
 * This function iterates through the linked list of redirection nodes,
 * freeing the file name string and the node itself for each redirection.
 *
 * @param r A pointer to the head of the redirection list to free.
 */
void	free_redirections(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		free(r->file);
		free(r);
		r = next;
	}
}
