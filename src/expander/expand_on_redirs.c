/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:35:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 11:17:51 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/**
 * @brief Expands variables in a single redirection.
 *
 * This function performs variable expansion on a redirection's file name
 * if expansion is appropriate for that redirection type.
 *
 * @param redir The redirection to expand.
 * @param env The environment variables list.
 * @param exit_status The last command's exit status.
 * @return 1 on success, 0 on failure.
 */
static int	expand_single_redir(t_redir *redir, t_env *env, int exit_status)
{
	char	*expanded;

	if (redir->type == REDIR_HEREDOC)
		return (1);
	expanded = expand_string(redir->file, env, exit_status);
	if (!expanded)
		return (0);
	free(redir->file);
	redir->file = expanded;
	return (1);
}

/**
 * @brief Expands variables in all redirections of a command.
 *
 * This function iterates through all redirections of a command and
 * performs variable expansion on each appropriate redirection.
 *
 * @param cmd The command whose redirections should be expanded.
 * @param env The environment variables list.
 * @param exit_status The last command's exit status.
 * @return 1 on success, 0 on failure.
 */
int	expand_on_redirs(t_command *cmd, t_env *env, int exit_status)
{
	t_redir	*redir;

	if (!cmd)
		return (1);
	redir = cmd->redir;
	while (redir)
	{
		if (!expand_single_redir(redir, env, exit_status))
			return (0);
		redir = redir->next;
	}
	return (1);
}
