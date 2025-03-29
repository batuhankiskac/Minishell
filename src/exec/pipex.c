/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:26:37 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/29 20:38:16 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	allpath = ft_split(ft_getenv("PATH", envp), ':');
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

void	child_process(int infile, int pipe_fd[2], char *cmd, char *envp[])
{
	close(pipe_fd[0]);
	dup2(infile, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(infile);
	close(pipe_fd[1]);
	execute_cmd(cmd, envp);
}

void	parent_process(int outfile, int pipe_fd[2], char *cmd, char *envp[])
{
	close(pipe_fd[1]);
	dup2(outfile, STDOUT_FILENO);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(outfile);
	close(pipe_fd[0]);
	execute_cmd(cmd, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		infile;
	int		outfile;
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: /pipex file1 cmd1 cmd2 file2\n", 2);
		return (EXIT_FAILURE);
	}
	infile = open_file(argv[1], O_RDONLY, 0);
	outfile = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipe(pipe_fd) == -1)
		show_error("Error creating pipe");
	pid = fork();
	if (pid == -1)
		show_error("Error creating child process");
	if (pid == 0)
		child_process(infile, pipe_fd, argv[2], envp);
	else
	{
		waitpid(pid, NULL, 0);
		parent_process(outfile, pipe_fd, argv[3], envp);
	}
	return (EXIT_SUCCESS);
}
