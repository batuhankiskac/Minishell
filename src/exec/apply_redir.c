/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:31:39 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/18 14:41:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles input redirection (<).
 *
 * This function opens the specified file for reading and duplicates its file
 * descriptor to standard input (fd 0). If either operation fails, it returns
 * ERROR.
 *
 * @param redir A pointer to the redirection structure containing the file name.
 * @return 0 on success, ERROR on failure.
 */
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

/**
 * @brief Handles output redirection (>).
 *
 * This function opens the specified file for writing, creating it if it doesn't
 * exist and truncating it if it does. It then duplicates the file descriptor
 * to standard output (fd 1). If either operation fails, it returns ERROR.
 *
 * @param redir A pointer to the redirection structure containing the file name.
 * @return 0 on success, ERROR on failure.
 */
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

/**
 * @brief Handles append redirection (>>).
 *
 * This function opens the specified file for writing in append mode, creating
 * it if it doesn't exist. It then duplicates the file descriptor to standard
 * output (fd 1). If either operation fails, it returns ERROR.
 *
 * @param redir A pointer to the redirection structure containing the file name.
 * @return 0 on success, ERROR on failure.
 */
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
 * This function examines the type of redirection and calls the appropriate
 * handler function. It supports input redirection (<), output redirection (>),
 * and append redirection (>>). Heredoc redirection is handled separately
 * before command execution.
 *
 * @param redir A pointer to the redirection structure to process.
 * @return 0 on success, ERROR on failure.
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
