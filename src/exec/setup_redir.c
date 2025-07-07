/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 23:35:34 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds the last heredoc redirection in a redirection list.
 *
 * This function iterates through a redirection list and returns a pointer
 * to the last heredoc redirection found, or NULL if none exists.
 *
 * @param redir A pointer to the first redirection in the list.
 * @return A pointer to the last heredoc redirection, or NULL if none found.
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
 * @brief Processes a heredoc redirection.
 *
 * This function handles heredoc redirections by determining whether to
 * process them normally or collect them only, based on whether it's
 * the last heredoc in the list.
 *
 * @param shell A pointer to the shell structure.
 * @param redir A pointer to the current redirection.
 * @param last_heredoc A pointer to the last heredoc in the list.
 * @return 0 on success, ERROR on failure.
 */
static int	process_heredoc(t_shell *shell, t_redir *redir,
		t_redir *last_heredoc)
{
	int	ret;

	shell->redir = redir;
	if (redir == last_heredoc)
	{
		ret = handle_heredoc_redir(shell);
		if (ret == 1)
		{
			shell->heredoc_eof = 1;
			return (ERROR);
		}
		if (ret == ERROR)
			return (ERROR);
	}
	else
	{
		ret = handle_heredoc_collect_only(shell);
		if (ret == ERROR)
			return (ERROR);
	}
	return (0);
}

/**
 * @brief Processes a single redirection.
 *
 * This function handles a single redirection, either heredoc or regular
 * redirection types (input, output, append).
 *
 * @param shell A pointer to the shell structure.
 * @param redir A pointer to the current redirection.
 * @param last_heredoc A pointer to the last heredoc in the list.
 * @return 0 on success, ERROR on failure.
 */
static int	process_single_redir(t_shell *shell, t_redir *redir,
		t_redir *last_heredoc)
{
	int	ret;

	if (redir->type == REDIR_HEREDOC)
	{
		ret = process_heredoc(shell, redir, last_heredoc);
		if (ret == ERROR)
			return (ERROR);
	}
	else
	{
		ret = apply_redirection(shell, redir);
		if (ret == ERROR)
			return (ERROR);
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
 *   file descriptor to `0`.
 * - For `REDIR_OUT`, it opens (or creates/truncates) the specified file for
 *   writing and duplicates its file descriptor to `1`.
 * - For `REDIR_APPEND`, it opens (or creates/appends to) the specified file for
 *   writing and duplicates its file descriptor to `1`.
 * - For `REDIR_HEREDOC`, it calls `handle_heredoc_redir` to manage the
 *   here-document input, which typically involves reading lines until a
 *   delimiter and making that input available on `0`.
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
	t_redir	*last_heredoc;
	int		ret;

	if (!shell || !shell->command)
		return (0);
	redir = shell->command->redir;
	if (!redir)
		return (0);
	last_heredoc = find_last_heredoc(redir);
	while (redir)
	{
		ret = process_single_redir(shell, redir, last_heredoc);
		if (ret == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
