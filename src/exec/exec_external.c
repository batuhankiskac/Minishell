/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:43:58 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 22:00:00 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
