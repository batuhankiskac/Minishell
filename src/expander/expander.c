/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/02 23:13:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands variables and redirections in the command arguments and
 * redirections.
 *
 * This function iterates through each command in the shell's command list,
 * expanding variables in the command arguments and redirections using the
 * provided environment and exit status.
 *
 * @param shell A pointer to the `t_shell` structure containing the command list,
 *              environment, and exit status.
 * @return 1 on success, 0 on failure (e.g., if expansion fails).
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
