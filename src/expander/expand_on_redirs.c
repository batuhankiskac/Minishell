/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:08:33 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands the file paths in the redirection list of a command.
 *
 * This function iterates through the redirection list of a command and
 * expands each file path using the `expand_string` function. It updates
 * the `file` field of each redirection node with the expanded value.
 *
 * @param cmd A pointer to the command structure containing the redirections.
 * @param env A pointer to the environment structure used for variable expansion.
 * @param exit_status The exit status of the last executed command, used in
 * expansions.
 * @return 1 on success, 0 if an error occurs during expansion.
 */
int	expand_on_redirs(t_command *cmd, t_env *env, int exit_status)
{
	t_redir	*r;
	char	*new;

	r = cmd->redir;
	while (r)
	{
		new = expand_string(r->file, env, exit_status);
		if (!new)
			return (0);
		free(r->file);
		r->file = new;
		r = r->next;
	}
	return (1);
}
