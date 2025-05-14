/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:43:21 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:43:46 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * expander - runs variable expansion on all commands
 *
 * Walks the built command list and for each node:
 *   1) expands all args via expand_on_args()
 *   2) expands all redir filenames via expand_on_redirs()
 *
 * @shell: shell context with .command, .env, .exit_status populated
 * Returns 1 on success, 0 on malloc error
 */
int	expander(t_shell *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	while (cmd)
	{
		if (!expand_on_args(cmd, shell->env, shell->exit_status))
			return (0);
		if (!expand_on_redirs(cmd, shell->env, shell->exit_status))
			return (0);
		cmd = cmd->next;
	}
	return (1);
}
