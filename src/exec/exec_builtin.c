/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 07:15:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes the appropriate built-in command.
 *
 * This function identifies and calls the correct built-in command
 * based on the command name.
 *
 * @param shell A pointer to the shell structure.
 * @return The exit status of the executed built-in command.
 */
static int	execute_builtin_command(t_shell *shell)
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

/**
 * @brief Executes a built-in command with robust redirection and cleanup.
 *
 * This function orchestrates the execution of built-in commands. It handles
 * redirection by safely backing up original file descriptors (stdin/stdout),
 * attempting to apply the command's redirections, and *always* restoring
 * the original descriptors using the dup_fd helper, regardless of whether
 * the redirection succeeded or failed. This prevents the main shell's file
 * descriptors from being left in a corrupted state after a redirection error.
 *
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns the exit status of the command. Returns 1 on redirection
 * failure, otherwise returns the exit status from the builtin itself.
 */
int	exec_builtin(t_shell *shell)
{
	int	original_stdout;
	int	original_stdin;
	int	result;

	original_stdout = -1;
	original_stdin = -1;
	result = 0;
	if (shell->command && shell->command->redir)
	{
		original_stdin = dup(STDIN_FILENO);
		original_stdout = dup(STDOUT_FILENO);
		if (setup_redir(shell) == ERROR)
			result = 1;
	}
	if (result == 0)
		result = execute_builtin_command(shell);
	if (original_stdin != -1)
		dup_fd(original_stdin, STDIN_FILENO, "stdin");
	if (original_stdout != -1)
		dup_fd(original_stdout, STDOUT_FILENO, "stdout");
	return (result);
}
