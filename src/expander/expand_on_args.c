/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 19:28:52 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_on_args(t_command *cmd, t_env *env, int exit_status)
{
	int		i;
	char	*new_val;
	
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new_val = expand_string(cmd->args[i], env, exit_status);
		if (!new_val)
			return (0);
		free(cmd->args[i]);
		cmd->args[i] = new_val;

		if (i == 0)
		{
			cmd->cmd = cmd->args[0];
		}
		i++;
	}
	return (1);
}

