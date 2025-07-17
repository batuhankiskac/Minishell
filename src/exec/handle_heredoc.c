/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/17 18:08:57 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child_process(t_shell *shell, t_redir *redir,
	int pipe_fd[2])
{
	int exit_code;

	close(pipe_fd[0]);
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	reset_signal_flag();
	heredoc_read_loop(shell, redir, pipe_fd[1]);
	close(pipe_fd[1]);

	exit_code = 0;
	// After the loop, check if the signal flag was set.
	if (get_signal_flag() == SIGINT)
	{
		// If so, prepare to exit with status 130.
		exit_code = 130;
	}

	// Always call cleanup before exiting.
	cleanup_child_and_exit(shell, NULL, NULL, exit_code);
}

static int	heredoc_parent_process(pid_t pid, int pipe_fd[2], t_redir *redir)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	set_interactive_signals();

	// The child now exits voluntarily, so we check its exit status.
	// If the status is 130, we know it was interrupted by SIGINT.
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		write(1, "\n", 1);
		return (1);
	}

	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int		pipe_fd[2];
	pid_t	pid;

	redir->heredoc_fd = -1;
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (print_error(NULL, "fork", strerror(errno), ERROR));
	}

	if (pid == 0)
		heredoc_child_process(shell, redir, pipe_fd);
	else
		return (heredoc_parent_process(pid, pipe_fd, redir));

	return (0);
}

int	handle_heredoc_redir(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;
	int			result;

	cmd = shell->command;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				result = execute_single_heredoc(shell, redir);
				if (result != 0)
				{
					if (result == 1)
						shell->exit_status = 130;
					return (result);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
