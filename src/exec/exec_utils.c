/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
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

char	*find_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;
	allpath = ft_split(get_env("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	i = -1;
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_all(allpath);
			ft_free_all(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(allpath);
	ft_free_all(s_cmd);
	return (cmd);
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

