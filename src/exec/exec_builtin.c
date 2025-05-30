/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:59:36 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a built-in command.
 *
 * This function identifies and executes the appropriate built-in command
 * based on the command name provided in the `shell->command->cmd` field.
 * It supports `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`.
 * The function passes the necessary arguments and environment variables
 * to the respective built-in command functions.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              command to be executed, its arguments, and the shell's
 *              environment.
 * @return Returns the exit status of the executed built-in command. If the
 *         command is not a recognized built-in, it returns 0.
 */
int	exec_builtin(t_shell *shell)
{
	if (ft_strcmp(shell->command->cmd, "cd") == 0)
		return (builtin_cd(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "echo") == 0)
		return (builtin_echo(shell->command->argc, shell->command->args));
	else if (ft_strcmp(shell->command->cmd, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(shell->command->cmd, "export") == 0)
		return (builtin_export(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "unset") == 0)
		return (builtin_unset(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "env") == 0)
		return (builtin_env(shell->env));
	else if (ft_strcmp(shell->command->cmd, "exit") == 0)
		return (builtin_exit(shell));
	return (0);
}
