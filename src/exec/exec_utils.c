/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 22:12:59 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *name, char *envp[])
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	open_file(char *filename, int flags, int mode, char *type)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: open ", 2);
		ft_putstr_fd(type, 2);
		ft_putstr_fd(" file", 2);
		perror(filename);
		return (ERROR);
	}
	return (fd);
}

int	dup_fd(int old_fd, int new_fd, char *type)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		ft_putstr_fd("minishell: dup2 ", 2);
		perror(type);
		close(old_fd);
		return (ERROR);
	}
	close(old_fd);
	return (0);
}

void	close_pipe_fd(int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1)
		close(prev_fd);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}
