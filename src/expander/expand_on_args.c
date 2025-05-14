/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:34:51 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:34:52 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * expand_on_args - expand each argument in a command’s args[]
 *
 * @cmd:          pointer to the command whose args will be processed
 * @env:          head of the environment variables list
 * @exit_status:  last command’s exit status for `$?` expansion
 *
 * Returns 1 on success, 0 on malloc error.
 */
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
