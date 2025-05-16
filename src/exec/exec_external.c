/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:37:29 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child_process(t_shell *shell, char **env_array)
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

int	exec_external(t_shell *shell)
{
	int		status;
	char	**env_array;
	pid_t	pid;
	int		validation_result;

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
