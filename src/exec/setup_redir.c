/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 17:00:17 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_RDONLY, 0, "input");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDIN_FILENO, "input") == ERROR)
		return (ERROR);
	return (0);
}

static int	handle_output_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644, "output");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "output") == ERROR)
		return (ERROR);
	return (0);
}

static int	handle_append_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644, "append");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "append") == ERROR)
		return (ERROR);
	return (0);
}

/**
 * @brief Processes a single redirection based on its type.
 *
 * @param shell Shell structure (needed for HEREDOC)
 * @param redir The redirection to process
 * @return 0 on success, ERROR on failure
 */
static int	process_single_redir(t_shell *shell, t_redir *redir)
{
	int	result;

	if (redir->type == REDIR_IN)
		return (handle_input_redir(redir));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redir(redir));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redir(redir));
	else if (redir->type == REDIR_HEREDOC)
	{
		shell->redir = redir;
		result = handle_heredoc_redir(shell);
		if (result == 1)
		{
			shell->heredoc_eof = 1;
			return (ERROR);
		}
		return (result);
	}
	return (0);
}

/**
 * @brief Sets up input and output redirections for a command.
 *
 * This function iterates through the list of redirections
 * (`shell->command->redir`)
 * associated with the current command. For each redirection, it performs the
 * necessary actions based on the redirection type (`REDIR_IN`, `REDIR_OUT`,
 * `REDIR_APPEND`, `REDIR_HEREDOC`).
 *
 * - For `REDIR_IN`, it opens the specified file for reading and duplicates its
 *   file descriptor to `STDIN_FILENO`.
 * - For `REDIR_OUT`, it opens (or creates/truncates) the specified file for
 *   writing and duplicates its file descriptor to `STDOUT_FILENO`.
 * - For `REDIR_APPEND`, it opens (or creates/appends to) the specified file for
 *   writing and duplicates its file descriptor to `STDOUT_FILENO`.
 * - For `REDIR_HEREDOC`, it calls `handle_heredoc_redir` to manage the
 *   here-document input, which typically involves reading lines until a
 *   delimiter and making that input available on `STDIN_FILENO`.
 *
 * If any `open` or `dup2` operation fails, or if `handle_heredoc_redir` fails,
 * an error message is printed, and the function returns `ERROR`. Otherwise, it
 * returns 0 after processing all redirections.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              current command and its associated redirections.
 * @return Returns 0 on successful setup of all redirections. Returns `ERROR`
 *         if any redirection operation fails.
 */
int	setup_redir(t_shell *shell)
{
	t_redir	*redir;
	int		ret;

	if (!shell || !shell->command)
		return (0);
	redir = shell->command->redir;
	if (!redir)
		return (0);
	while (redir)
	{
		ret = process_single_redir(shell, redir);
		if (ret == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
