/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 22:02:26 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes the command using execve.
 *
 * This function finds the command's executable path and executes it using
 * execve. If the command is not found or execution fails, an error message
 * is printed and the child process exits with the appropriate status code.
 *
 * @param shell A pointer to the shell structure containing the command.
 * @param env_array An array of environment variables for the child process.
 */
static void	execute_command(t_shell *shell, char **env_array)
{
	char	*path;

	path = find_path(shell->command->cmd, env_array);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(shell->command->cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	if (execve(path, shell->command->args, env_array) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(shell->command->cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		if (ft_strcmp(path, shell->command->cmd) != 0)
			free(path);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(path, shell->command->cmd) != 0)
		free(path);
	exit(EXIT_SUCCESS);
}

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
	{
		ft_putendl_fd("minishell: redirection setup failed", STDERR_FILENO);
		exit(1);
	}
	execute_command(shell, env_array);
}

/**
 * @brief Validates the command structure before execution.
 *
 * This function checks if the command structure in the shell is valid.
 * It ensures that the command and its arguments are not NULL and that
 * the command string is not empty. If validation fails, an error message
 * is printed, and an appropriate error code is returned.
 *
 * @param shell A pointer to the shell structure containing the command.
 * @param env_array An array of environment variables for cleanup if needed.
 * @return 0 if the command is valid, or an error code if validation fails.
 */
static int	validate_command(t_shell *shell, char **env_array)
{
	if (!shell->command || !shell->command->cmd || !shell->command->args)
	{
		ft_putendl_fd("minishell: command not found", STDERR_FILENO);
		ft_free_all(env_array);
		return (127);
	}
	if (shell->command->cmd[0] == '\0')
	{
		ft_putendl_fd("minishell: command not found", STDERR_FILENO);
		ft_free_all(env_array);
		return (127);
	}
	return (0);
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
	int		status;
	char	**env_array;
	pid_t	pid;
	int		validation_result;

	status = 0;
	env_array = env_list_to_array(shell->env);
	if (!env_array)
		return (perror("minishell: env_list_to_array"), ERROR);
	validation_result = validate_command(shell, env_array);
	if (validation_result != 0)
		return (validation_result);
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), ft_free_all(env_array), ERROR);
	if (pid == 0)
		execute_child_process(shell, env_array);
	else
		waitpid(pid, &status, 0);
	ft_free_all(env_array);
	return (WEXITSTATUS(status));
}
