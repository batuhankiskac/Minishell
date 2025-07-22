/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rebuild_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 07:24:25 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/22 09:15:44 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Rebuilds the command's argument list to handle leading empty args.
 *
 * After expansion, an unquoted empty variable at the start of a command
 * (e.g., "$EMPTY echo hi") results in an empty string as the first argument.
 * This function detects this specific case and shifts the arguments to the
 * left, making the next argument the actual command. This mimics bash's
 * behavior of removing null expansions without affecting legitimate quoted
 * empty strings ("") as arguments to other commands.
 *
 * @param cmd The command to process.
 * @return 1 on success, 0 on failure (malloc error).
 */
int	rebuild_command_args(t_command *cmd)
{
	char	**new_args;
	int		i;

	if (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] != '\0')
		return (1);
	if (cmd->argc <= 1)
		return (1);
	new_args = ft_calloc(cmd->argc, sizeof(char *));
	if (!new_args)
		return (print_error(NULL, NULL, strerror(errno), 0));
	i = 0;
	while (cmd->args[i + 1])
	{
		new_args[i] = cmd->args[i + 1];
		i++;
	}
	free(cmd->args[0]);
	free(cmd->args);
	cmd->args = new_args;
	cmd->argc = cmd->argc - 1;
	cmd->cmd = cmd->args[0];
	return (1);
}
