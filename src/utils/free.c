/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 17:28:48 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Safely frees a null-terminated array of strings.
 *
 * This function iterates through the array of strings, freeing each
 * string, and then frees the array itself. Includes safety checks
 * to prevent double-free and handles NULL pointers gracefully.
 *
 * @param arr A null-terminated array of strings to free.
 */
void	ft_free_all(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		if (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		i++;
	}
	free(arr);
}

/**
 * @brief Safely frees a linked list of redirection nodes.
 *
 * This function iterates through the linked list of redirection nodes,
 * freeing the file name strings and the node itself for each redirection.
 * Includes safety checks to prevent double-free and handles NULL pointers.
 *
 * @param r A pointer to the head of the redirection list to free.
 */
void	free_redirections(t_redir *r)
{
	t_redir	*next;

	if (!r)
		return ;
	while (r)
	{
		next = r->next;
		if (r->file)
		{
			free(r->file);
			r->file = NULL;
		}
		if (r->original_file)
		{
			free(r->original_file);
			r->original_file = NULL;
		}
		free(r);
		r = next;
	}
}

/**
 * @brief Safely frees an array of heredoc lines.
 *
 * This function frees each line in the array and then frees the array itself.
 * Includes safety checks to prevent double-free, validates count parameter,
 * and handles NULL pointers gracefully.
 *
 * @param lines Array of lines to free
 * @param count Number of lines (must be >= 0)
 */
void	free_heredoc(char **lines, int count)
{
	int	i;

	if (!lines || count < 0)
		return ;
	i = 0;
	while (i < count && lines[i])
	{
		free(lines[i]);
		lines[i] = NULL;
		i++;
	}
	free(lines);
}
