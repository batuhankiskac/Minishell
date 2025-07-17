/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/17 21:31:46 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets up redirection for built-in commands.
 *
 * This function backs up the original file descriptors and applies
 * redirections if they exist for the command.
 *
 * @param shell A pointer to the shell structure.
 * @param original_stdout Pointer to store the backup of stdout.
 * @param original_stdin Pointer to store the backup of stdin.
 * @return 0 on success, ERROR on failure.
 */
static int	setup_builtin_redir(t_shell *shell, int *original_stdout,
									int *original_stdin)
{
	*original_stdout = -1;
	*original_stdin = -1;
	if (shell->command && shell->command->redir)
	{
		*original_stdout = dup(1);
		*original_stdin = dup(0);
		if (setup_redir(shell) == ERROR)
		{
			if (*original_stdout != -1)
				close(*original_stdout);
			if (*original_stdin != -1)
				close(*original_stdin);
			return (ERROR);
		}
	}
	return (0);
}

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
 * @brief Restores original file descriptors after built-in execution.
 *
 * This function restores the original stdout and stdin file descriptors
 * that were backed up before redirection was applied.
 *
 * @param original_stdout The backup of the original stdout.
 * @param original_stdin The backup of the original stdin.
 */
static void	restore_builtin_fds(int original_stdout, int original_stdin)
{
	if (original_stdout != -1)
		dup_fd(original_stdout, 1, "stdout");
	if (original_stdin != -1)
		dup_fd(original_stdin, 0, "stdin");
}

/**
 * @brief Executes a built-in command with redirection support.
 *
 * This function orchestrates the execution of built-in commands by
 * setting up redirections, executing the command, and restoring
 * file descriptors.
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

	if (setup_builtin_redir(shell, &original_stdout, &original_stdin) == ERROR)
	{
		restore_builtin_fds(original_stdout, original_stdin);
		return (1);
	}
	result = execute_builtin_command(shell);
	restore_builtin_fds(original_stdout, original_stdin);
	return (result);
}
