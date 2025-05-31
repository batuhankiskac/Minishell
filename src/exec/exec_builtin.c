/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/31 16:45:27 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a built-in command with redirection support.
 *
 * This function identifies and executes the appropriate built-in command
 * based on the command name provided in the `shell->command->cmd` field.
 * It supports `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`.
 * The function handles redirection by backing up original file descriptors,
 * applying redirections, executing the command, and restoring file descriptors.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              command to be executed, its arguments, and the shell's
 *              environment.
 * @return Returns the exit status of the executed built-in command. If the
 *         command is not a recognized built-in, it returns 0.
 */
int	exec_builtin(t_shell *shell)
{
	int	original_stdout;
	int	original_stdin;
	int	result;

	original_stdout = -1;
	original_stdin = -1;
	if (shell->command && shell->command->redir)
	{
		original_stdout = dup(STDOUT_FILENO);
		original_stdin = dup(STDIN_FILENO);
		if (setup_redir(shell) == ERROR)
		{
			if (original_stdout != -1)
				close(original_stdout);
			if (original_stdin != -1)
				close(original_stdin);
			return (ERROR);
		}
	}
	if (ft_strcmp(shell->command->cmd, "cd") == 0)
		result = builtin_cd(shell->command->argc,
				shell->command->args, &shell->env);
	else if (ft_strcmp(shell->command->cmd, "echo") == 0)
		result = builtin_echo(shell->command->argc, shell->command->args);
	else if (ft_strcmp(shell->command->cmd, "pwd") == 0)
		result = builtin_pwd();
	else if (ft_strcmp(shell->command->cmd, "export") == 0)
		result = builtin_export(shell->command->argc,
				shell->command->args, &shell->env);
	else if (ft_strcmp(shell->command->cmd, "unset") == 0)
		result = builtin_unset(shell->command->argc,
				shell->command->args, &shell->env);
	else if (ft_strcmp(shell->command->cmd, "env") == 0)
		result = builtin_env(shell->env);
	else if (ft_strcmp(shell->command->cmd, "exit") == 0)
		result = builtin_exit(shell);
	else
		result = 0;
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	return (result);
}
