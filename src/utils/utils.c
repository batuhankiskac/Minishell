/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 11:12:59 by bkiskac          ###   ########.fr       */
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
		free(r->original_file);
		free(r);
		r = next;
	}
}

/**
 * @brief Prints a standardized error message to STDERR.
 * @param cmd The command that caused the error (e.g., "cd").
 * @param arg The argument related to the error (can be NULL).
 * @param msg The specific error message.
 * @param err_code The error code to return.
 * @return Returns the specified error code.
 */
int	print_error(char *cmd, char *arg, char *msg, int err_code)
{
	ft_printf(2, "minishell: ");
	if (cmd)
		ft_printf(2, "%s: ", cmd);
	if (arg)
		ft_printf(2, "%s: ", arg);
	ft_printf(2, "%s\n", msg);
	return (err_code);
}

/**
 * @brief Prints error message and returns NULL pointer.
 * @param cmd The command that caused the error.
 * @param arg The argument related to the error (can be NULL).
 * @param msg The specific error message.
 * @return Always returns NULL.
 */
void	*print_error_null(char *cmd, char *arg, char *msg)
{
	print_error(cmd, arg, msg, 0);
	return (NULL);
}
