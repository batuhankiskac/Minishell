/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:16:06 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 15:11:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* exec_builtin - Executes a built-in shell command.
*
* This function identifies and executes the appropriate
* built-in command based on
* the command name in the shell structure.
* It supports the following built-ins:
*   - echo: Display a message on standard output
*   - cd: Change the current directory
*   - pwd: Print the current working directory
*   - export: Set environment variables
*   - unset: Remove environment variables
*   - env: Display the environment variables
*   - exit: Exit the shell with a status code
* Parameters:
*   t_shell *shell - A pointer to the shell structure
* containing command info and environment variables.
* Returns:
*   The exit status of the executed built-in command (0 for success,
*   non-zero for failure).
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
		return (builtin_exit(shell->command->argc,
				shell->command->args, &shell->env));
	return (0);
}
