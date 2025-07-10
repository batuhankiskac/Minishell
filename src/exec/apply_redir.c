/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:31:39 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 08:12:23 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_RDONLY, 0);
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, 0, "input") == ERROR)
		return (ERROR);
	return (0);
}

static int	handle_output_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, 1, "output") == ERROR)
		return (ERROR);
	return (0);
}

static int	handle_append_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, 1, "append") == ERROR)
		return (ERROR);
	return (0);
}

/**
 * @brief Processes a single redirection based on its type.
 *
 * @param redir The redirection to process
 * @return 0 on success, ERROR on failure
 */
int	apply_redirection(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (handle_input_redir(redir));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redir(redir));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redir(redir));
	return (0);
}
