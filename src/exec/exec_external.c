/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 19:02:23 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a command in the child process.
 *
 * This function is called in the child process after a fork. It sets up
 * redirections and then executes the command. If redirection setup fails,
 * the child process exits with an error code.
 *
 * @param shell A pointer to the shell structure containing the command
 *              and its arguments.
 * @param env_array An array of environment variables for the child process.
 */
static void	execute_child_process(t_shell *shell, char **env_array)
{
	reset_signals();
	if (setup_redir(shell) == ERROR)
		cleanup_child_and_exit(shell, env_array, NULL, 1);
	find_and_exec_command(shell, env_array);
}

/**
 * @brief Forks the process and executes the command in the child.
 *
 * This function forks the current process. The child process executes the
 * command, and the parent process waits for the child to complete.
 * The exit status of the child is handled and returned.
 *
 * @param shell A pointer to the shell structure.
 * @param env_array An array of environment variables.
 * @return The exit status of the command.
 */
static int	fork_and_execute(t_shell *shell, char **env_array)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		ft_free_all(env_array);
		return (print_error(NULL, NULL, strerror(errno), 1));
	}
	if (pid == 0)
		execute_child_process(shell, env_array);
	else
	{
		waitpid(pid, &status, 0);
		return (handle_wait_status(status, env_array));
	}
	return (status);
}

/**
 * @brief Executes an external command.
 *
 * This function handles the execution of external commands by forking
 * a child process. The child process attempts to execute the command
 * using execve, while the parent process waits for the child to finish.
 *
 * @param shell A pointer to the shell structure containing the command
 *              and its arguments.
 * @return The exit status of the executed command, or an error code if
 *         the execution fails.
 */
int	exec_external(t_shell *shell)
{
	int		validation_result;
	char	**env_array;

	env_array = env_list_to_array(shell->env);
	if (!env_array)
		return (print_error(NULL, NULL, strerror(errno), 1));
	validation_result = validate_command(shell, env_array);
	if (validation_result != 0)
		return (validation_result);
	return (fork_and_execute(shell, env_array));
}

/**
 * @brief Executes an external command directly without forking.
 *
 * This function is used by pipeline child processes to execute external
 * commands directly using execve without creating another child process.
 * It validates the command, converts environment, finds the path,
 * and calls execve.
 * Memory cleanup is handled before exit on error cases.
 *
 * @param shell A pointer to the shell structure containing the command.
 * @param env_array An array of environment variables for the process.
 */
void	exec_external_direct(t_shell *shell, char **env_array)
{
	int	validation_result;

	validation_result = validate_command(shell, env_array);
	if (validation_result != 0)
		cleanup_child_and_exit(shell, env_array, NULL, validation_result);
	find_and_exec_command(shell, env_array);
	cleanup_child_and_exit(shell, env_array, NULL, EXIT_FAILURE);
}
