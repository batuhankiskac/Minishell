/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/11 19:24:37 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds the last here-document redirection in a redirection list.
 * @param redir A pointer to the head of the redirection list.
 * @return A pointer to the last heredoc (`t_redir` node) found, or NULL if
 * no heredoc redirections exist in the list.
 * @details This is a crucial helper because in a command with multiple
 * heredocs (e.g., `cat << EOF1 << EOF2`), only the input from the very
 * last one is actually passed to the command's stdin. The preceding
 * heredocs must still be processed to consume their input, but their
 * content is discarded.
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
 * @param shell A pointer to the shell structure.
 * @param redir The specific redirection node to process.
 * @param last_heredoc A pointer to the last heredoc in the list, needed
 * for context by the heredoc handler.
 * @return 0 on success, ERROR (-1) on failure.
 * @details This function acts as a dispatcher. It checks the type of the
 * redirection and calls the appropriate handler: `process_heredoc` for
 * here-documents or `apply_redirection` for standard file redirections
 * (`<`, `>`, `>>`).
 */
static int	process_single_redir(t_shell *shell, t_redir *redir,
		t_redir *last_heredoc)
{

	if (redir->type == REDIR_HEREDOC)
	{
		if (handle_heredoc_redir(shell, redir,
				(last_heredoc == redir)) == ERROR)
			return (ERROR);
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
 * @param shell A pointer to the `t_shell` structure, containing the
 * command whose redirections need to be processed.
 * @return Returns 0 on successful setup of all redirections.
 * Returns ERROR (-1) if any redirection operation fails or is interrupted.
 * @details This is the main entry point for redirection setup. It iterates
 * through the linked list of redirections associated with the current
 * command. Before the loop, it finds the `last_heredoc` to provide
 * necessary context. Inside the loop, it calls `process_single_redir` for
 * each node. If any step fails, it immediately stops and returns an error,
 * preventing the command from executing with a broken I/O setup.
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
		if (process_single_redir(shell, redir, last_heredoc) == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
