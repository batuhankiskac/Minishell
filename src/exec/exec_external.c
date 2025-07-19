/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 19:23:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a command in the child process.
 *
 * This function is called in the child process after a fork. It sets up
 * redirections and then executes the command. If redirection setup fails,
 * the child process exits with code 1.
 *
 * @param shell A pointer to the shell structure containing the command
 * and its arguments.
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
 * @brief Waits for the child process and handles its exit status.
 *
 * This function contains the parent's logic after a fork. It waits for the
 * specified child process to terminate, handling potential interruptions
 * from signals, and then returns the child's final exit status.
 *
 * @param pid The process ID of the child to wait for.
 * @param env_array The environment array to be freed after waiting.
 * @return The final exit status of the child command.
 */
static int	wait_for_child(pid_t pid, char **env_array)
{
	int		status;
	pid_t	waited_pid;

	status = 0;
	waited_pid = 0;
	while (waited_pid != pid)
	{
		waited_pid = waitpid(pid, &status, 0);
		if (waited_pid == -1 && errno == EINTR)
			continue ;
		else if (waited_pid == -1)
			break ;
	}
	return (handle_wait_status(status, env_array));
}

/**
 * @brief Forks the process and executes the command in the child.
 *
 * This function forks the current process. The child process executes the
 * command by calling execute_child_process, and the parent process waits
 * for the child to complete by calling wait_for_child.
 *
 * @param shell A pointer to the shell structure.
 * @param env_array An array of environment variables.
 * @return The exit status of the command.
 */
static int	fork_and_execute(t_shell *shell, char **env_array)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_free_all(env_array);
		return (print_error(NULL, NULL, strerror(errno), 1));
	}
	if (pid == 0)
		execute_child_process(shell, env_array);
	else
		return (wait_for_child(pid, env_array));
	return (1);
}

/**
 * @brief Executes an external command.
 *
 * This function handles the execution of external commands by forking
 * a child process. It validates the command and then calls the main
 * forking and execution logic.
 *
 * @param shell A pointer to the shell structure.
 * @return The exit status of the executed command.
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
 * It validates the command, finds the path, and calls execve.
 *
 * @param shell A pointer to the shell structure.
 * @param env_array An array of environment variables for the process.
 */
void	exec_external_direct(t_shell *shell, char **env_array)
{
	int	validation_result;

	validation_result = validate_command(shell, env_array);
	if (validation_result != 0)
		cleanup_child_and_exit(shell, env_array, NULL, validation_result);
	find_and_exec_command(shell, env_array);
	cleanup_child_and_exit(shell, env_array, NULL, 1);
}
