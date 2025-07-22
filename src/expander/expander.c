/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/22 22:06:02 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands variables and redirections, then cleans up empty arguments.
 *
 * This function iterates through each command, expanding variables in arguments
 * and redirections. Crucially, after expansion, it calls
 * rebuild_command_args to remove any arguments that became empty strings,
 * which mimics standard shell behavior for unquoted empty variables.
 *
 * @param shell A pointer to the `t_shell` structure.
 * @return 1 on success, 0 on failure.
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
