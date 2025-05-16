/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 19:29:49 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

