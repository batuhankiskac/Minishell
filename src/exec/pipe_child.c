/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:15:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 22:24:25 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipe_write_fd(t_command *cmd, int pipe_fd[2])
{
	int	pipe_write_fd;

	if (cmd->next)
	{
		pipe_write_fd = pipe_fd[1];
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
	}
	else
		pipe_write_fd = -1;
	return (pipe_write_fd);
}

static void	handle_input_redirection(int prev_fd)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 prev_fd");
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
}

static void	handle_output_redirection(int pipe_write_fd)
{
	if (pipe_write_fd != -1)
	{
		if (dup_fd(pipe_write_fd, STDOUT_FILENO, "pipe") == ERROR)
			exit(EXIT_FAILURE);
	}
}

static void	execute_child_cmd(t_shell *shell)
{
	if (setup_redir(shell) == ERROR)
		exit(EXIT_FAILURE);
	if (is_builtin(shell->command->cmd))
		exit(exec_builtin(shell));
	exit(exec_external(shell));
}

void	pipe_child_process(t_shell *shell,
	t_command *cmd, int prev_fd, int pipe_fd[2])
{
	int	pipe_write_fd;

	shell->command = cmd;
	pipe_write_fd = init_pipe_write_fd(cmd, pipe_fd);
	handle_input_redirection(prev_fd);
	handle_output_redirection(pipe_write_fd);
	execute_child_cmd(shell);
}
