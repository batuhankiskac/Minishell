/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:56:06 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/09 14:40:52 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Handles errors that occur after execve fails.
 * It checks the errno, prints a corresponding error message,
 * and exits the child process with an appropriate status code.
 * @param path The path that was attempted to be executed.
 * @param shell The main shell structure.
 * @param env_array The environment array for cleanup.
 */
static void	handle_exec_error(char *path, t_shell *shell, char **env_array)
{
	print_error(NULL, shell->command->cmd, strerror(errno), 0);
	if (ft_strcmp(path, shell->command->cmd) != 0)
		free(path);
	if (errno == EACCES)
		cleanup_child_and_exit(shell, env_array, NULL, 126);
	cleanup_child_and_exit(shell, env_array, NULL, EXIT_FAILURE);
}

/**
 * @brief      Finds the command path and executes the command.
 * This is the core function run within the child process for external commands.
 * It handles path resolution and execution.
 * @param shell The main shell structure.
 * @param env_array The environment variables for execve.
 */
void	find_and_exec_command(t_shell *shell, char **env_array)
{
	char	*path;
	char	*shell_path;
	char	*sh_args[3];

	path = find_path(shell->command->cmd, env_array);
	if (!path)
		cleanup_child_and_exit(shell, env_array, NULL,
			print_error(NULL, shell->command->cmd, "command not found", 127));
	if (execve(path, shell->command->args, env_array) == -1)
	{
		if (errno == ENOEXEC)
		{
			shell_path = "/bin/sh";
			sh_args[0] = "sh";
			sh_args[1] = path;
			sh_args[2] = NULL;
			execve(shell_path, sh_args, env_array);
		}
		handle_exec_error(path, shell, env_array);
	}
	if (ft_strcmp(path, shell->command->cmd) != 0)
		free(path);
	exit(EXIT_SUCCESS);
}
