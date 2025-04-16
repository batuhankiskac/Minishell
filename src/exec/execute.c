/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:54:31 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/16 09:43:50 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static void	update_parent_pipe()
{
	
}

static void	execute_pipe_child(t_shell *shell, int prev_fd, int pipe_write_fd)
{
	int	ret;

	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 prev_fd");
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
	if (pipe_write_fd != -1)
	{
		if (dup_fd(pipe_write_fd, STDOUT_FILENO, "pipe") == ERROR)
			exit(EXIT_FAILURE);
	}
	if (setup_redir(shell) == ERROR)
		exit(EXIT_FAILURE);
	if (is_builtin(shell->command->cmd))
		ret = exec_builtin(shell);
	else
		ret = exec_external(shell);
	exit(ret);
}

int	execute_pipe(t_shell *shell)
{
	t_command	*cmd;
	pid_t		pid;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;

	cmd = shell->command->cmd;
	prev_fd = -1;
	while (cmd)
	{
		if (setup_pipe_for_cmd(cmd, pipe_fd) == ERROR)
			return (ERROR);
		pid = fork();
		if (pid < 0)
			return (perror("minishell: fork"), ERROR);
		else if (pid == 0)
		{
			if (cmd->next)
				execute_pipe_child(shell, prev_fd, pipe_fd[1]);
			else
				execute_pipe_child(shell, prev_fd, -1);
		}
		else
			update_parent_pipe();
		cmd = cmd->next;
	}
}
