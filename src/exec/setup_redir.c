/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/17 21:12:59 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds the last here-document redirection in a redirection list.
 *
 * This is a crucial helper because in a command with multiple heredocs
 * (e.g., `cat << EOF1 << EOF2`), only the input from the very last one
 * is actually passed to the command's stdin.
 * @param redir A pointer to the head of the redirection list.
 * @return A pointer to the last heredoc (`t_redir` node), or NULL if none.
 */
static t_redir	*find_last_heredoc(t_redir *redir)
{
	t_redir	*last_heredoc;
	t_redir	*temp;

	last_heredoc = NULL;
	temp = redir;
	while (temp)
	{
		if (temp->type == REDIR_HEREDOC)
			last_heredoc = temp;
		temp = temp->next;
	}
	return (last_heredoc);
}

/**
 * @brief Processes a single redirection node from the command's list.
 *
 * This function acts as a dispatcher. It handles heredocs differently
 * from standard file redirections (`<`, `>`, `>>`).
 * @param redir The specific redirection node to process.
 * @param last_heredoc A pointer to the last heredoc in the list.
 * @return 0 on success, ERROR (-1) on failure.
 */
static int	process_single_redir(t_redir *redir, t_redir *last_heredoc)
{
	if (redir->type == REDIR_HEREDOC)
	{
		if (redir == last_heredoc)
		{
			if (dup_fd(redir->heredoc_fd, STDIN_FILENO, "heredoc") == ERROR)
				return (ERROR);
		}
		else
		{
			if (redir->heredoc_fd > 2)
				close(redir->heredoc_fd);
		}
	}
	else
	{
		if (apply_redirection(redir) == ERROR)
			return (ERROR);
	}
	return (0);
}

/**
 * @brief Sets up all input and output redirections for the current command.
 *
 * This is the main entry point for redirection setup. It iterates through
 * the list of redirections. For multiple heredocs, it ensures only the
 * last one's input is used and all others are properly closed.
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns 0 on success, or ERROR (-1) if any operation fails.
 */
int	setup_redir(t_shell *shell)
{
	t_redir	*redir;
	t_redir	*last_heredoc;

	if (!shell || !shell->command)
		return (0);
	redir = shell->command->redir;
	if (!redir)
		return (0);
	last_heredoc = find_last_heredoc(redir);
	while (redir)
	{
		if (process_single_redir(redir, last_heredoc) == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
