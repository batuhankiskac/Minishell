/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 21:37:06 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands the arguments of a command by replacing variables and
 * performing other expansions.
 *
 * This function iterates through the command's arguments, expanding each one
 * using the `expand_string` function. It updates the command's `cmd` field
 * with the first argument after expansion.
 *
 * @param cmd A pointer to the command structure containing the arguments to
 * expand.
 * @param env A pointer to the environment structure used for variable expansion.
 * @param exit_status The exit status of the last executed command, used in
 * expansions.
 * @return 1 on success, 0 if an error occurs during expansion.
 */
int	expand_on_args(t_command *cmd, t_env *env, int exit_status)
{
	int		i;
	int		arg_len;
	char	*new_val;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		arg_len = ft_strlen(cmd->args[i]);
		if (arg_len >= 2 && cmd->args[i][0] == '\''
				&& cmd->args[i][arg_len - 1] == '\'')
			new_val = ft_strndup(cmd->args[i] + 1, arg_len - 2);
		else
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
