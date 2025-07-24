/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:56:06 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/24 11:17:21 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Handles errors that occur after execve fails.
 *
 * @param path          The path that was attempted to be executed.
 * @param shell         The main shell structure.
 * @param env_array     The environment array for cleanup.
 * @param original_head The head of the command list for full cleanup.
 */
static void	handle_exec_error(char *path, t_shell *shell, char **env_array,
	t_command *original_head)
{
	print_error(NULL, shell->command->cmd, strerror(errno), 0);
	if (ft_strcmp(path, shell->command->cmd) != 0)
		free(path);
	if (errno == EACCES)
		cleanup_child_and_exit(shell, env_array, original_head, 126);
	cleanup_child_and_exit(shell, env_array, original_head, 1);
}

/**
 * @brief      Attempts to execute the command at the given path.
 *
 * This function handles the actual `execve` call. If `execve` fails, it
 * checks for the `ENOEXEC` error (e.g., a script without a shebang) and
 * attempts to run it with /bin/sh. For other errors, it calls a generic
 * error handler.
 *
 * @param path          The full path to the executable.
 * @param shell         The main shell structure.
 * @param env_array     The environment variables for execve.
 * @param original_head The head of the command list for full cleanup.
 */
static void	try_to_execute(char *path, t_shell *shell, char **env_array,
	t_command *original_head)
{
	char	*shell_path;
	char	*sh_args[3];

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
		handle_exec_error(path, shell, env_array, original_head);
	}
	if (ft_strcmp(path, shell->command->cmd) != 0)
		free(path);
	exit(0);
}

/**
 * @brief      Finds, validates, and executes a command.
 *
 * This function orchestrates the process of executing an external command. It
 * first finds the path, then validates permissions, and finally delegates
 * the actual execution attempt to the `try_to_execute` helper function. This
 * separation allows for cleaner, more focused error handling at each stage.
 *
 * @param shell         The main shell structure.
 * @param env_array     The environment variables for execve.
 * @param original_head The head of the command list for full cleanup.
 */
void	find_and_exec_command(t_shell *shell, char **env_array,
	t_command *original_head)
{
	char	*path;

	path = find_path(shell->command->cmd, env_array);
	if (!path)
	{
		cleanup_child_and_exit(shell, env_array, original_head,
			print_error(NULL, shell->command->cmd, "command not found", 127));
	}
	if (access(path, X_OK) != 0)
	{
		free(path);
		cleanup_child_and_exit(shell, env_array, original_head,
			print_error(NULL, shell->command->cmd, strerror(errno), 126));
	}
	try_to_execute(path, shell, env_array, original_head);
}
