/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 22:18:58 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_for_cmd(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
			return (perror("minishell: pipe"), ERROR);
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}

static void	update_parent_pipe(int *prev_fd, int pipe_fd[2], t_command *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		*prev_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
	else
		*prev_fd = -1;
}

static int	handle_pipe_iteration(t_shell *shell, t_command *cmd, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (setup_pipe_for_cmd(cmd, pipe_fd) == ERROR)
		return (ERROR);
	pid = fork();
	if (pid < 0)
		return (close_pipe_fd(*prev_fd, pipe_fd),
			perror("minishell: fork"), ERROR);
	if (pid == 0)
		pipe_child_process(shell, cmd, *prev_fd, pipe_fd);
	else
		update_parent_pipe(prev_fd, pipe_fd, cmd);
	return (0);
}

int	execute_pipe(t_shell *shell)
{
	t_command	*cmd;
	int			prev_fd;
	int			status;
	int			ret;

	prev_fd = -1;
	cmd = shell->command;
	while (cmd)
	{
		ret = handle_pipe_iteration(shell, cmd, &prev_fd);
		if (ret == ERROR)
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (ERROR);
		}
		cmd = cmd->next;
	}
	while (wait(&status) > 0)
		;
	return (WEXITSTATUS(status));
}
