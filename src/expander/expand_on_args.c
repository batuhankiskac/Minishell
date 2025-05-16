/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:18:36 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_on_args(t_command *cmd, t_env *env, int exit_status)
{
	int		i;
	char	*new;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new = expand_string(cmd->args[i], env, exit_status);
		if (!new)
			return (0);
		free(cmd->args[i]);
		cmd->args[i] = new;
		i++;
	}
	return (1);
}

