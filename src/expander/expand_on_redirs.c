/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:35:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 13:14:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a redirection should be skipped during expansion.
 *
 * This function determines whether a redirection should be excluded from
 * variable expansion based on its type and delimiters.
 *
 * @param redir The redirection node to check.
 * @return 1 if the redirection should be skipped, 0 otherwise.
 */
static int	should_skip_expansion(t_redir *redir)
{
	char	*file;

	if (redir->type != REDIR_HEREDOC)
		return (0);
	file = redir->original_file;
	if (!file)
		return (0);
	if ((file[0] == '\'' && file[ft_strlen(file) - 1] == '\'')
		|| (file[0] == '\"' && file[ft_strlen(file) - 1] == '\"'))
		return (1);
	return (0);
}

/**
 * @brief Expands variables in a single redirection.
 *
 * This function performs variable expansion on a redirection's file name
 * if expansion is appropriate for that redirection type.
 *
 * @param redir The redirection to expand.
 * @param env The environment variables list.
 * @param exit_status The last command's exit status.
 * @return 0 on success, ERROR on failure.
 */
static int	expand_single_redir(t_redir *redir, t_env *env, int exit_status)
{
	char	*expanded;

	if (should_skip_expansion(redir))
		return (0);
	expanded = expand_string(redir->file, env, exit_status);
	if (!expanded)
		return (ERROR);
	free(redir->file);
	redir->file = expanded;
	return (0);
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
 * @return 0 on success, ERROR on failure.
 */
int	expand_on_redirs(t_command *cmd, t_env *env, int exit_status)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	redir = cmd->redir;
	while (redir)
	{
		if (expand_single_redir(redir, env, exit_status) == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
