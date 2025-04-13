/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:43:58 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 13:33:07 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * child_exec - Executes an external command in a child process.
 *
 * This function attempts to execute an external command by:
 *   - Finding the full path of the command using the PATH environment variable.
 *   - Executing the command with the provided arguments and environment.
 *   - Handling errors if the command cannot be executed.
 *   - Freeing allocated memory for the path if necessary.
 *
 * Parameters:
 *   t_shell *shell - A pointer to the shell structure containing command info.
 *   char **env_array - An array of environment variable strings.
 *
 * Returns:
 *   Does not return if successful. Exits with failure status code if an error occurs.
 */
void	child_exec(t_shell *shell, char **env_array)
{
	char	*path;

	path = find_path(shell->command->cmd, env_array);
	if (execve(path, shell->command->args, env_array) == -1)
	{
		perror("minishell: execve");
		if (ft_strcmp(path, shell->command->cmd) != 0)
			free(path);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/*
 * exec_external - Handles execution of external commands.
 *
 * This function manages the execution of external commands by:
 *   - Converting the environment linked list to an array for execve.
 *   - Creating a child process using fork.
 *   - Having the child process execute the command.
 *   - Waiting for the child process to complete in the parent.
 *   - Cleaning up allocated resources.
 *   - Returning the exit status of the child process.
 *
 * Parameters:
 *   t_shell *shell - A pointer to the shell structure containing command and environment info.
 *
 * Returns:
 *   The exit status of the executed command, or ERROR if execution fails.
 */
int	exec_external(t_shell *shell)
{
	int		status;
	char	**env_array;
	pid_t	pid;

	env_array = env_list_to_array(shell->env);
	if (!env_array)
		return (perror("minishell: env_list_to_array"), ERROR);
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), ft_free_all(env_array), ERROR);
	if (pid == 0)
		child_exec(shell, env_array);
	else
		waitpid(pid, &status, 0);
	ft_free_all(env_array);
	return (WEXITSTATUS(status));
}
