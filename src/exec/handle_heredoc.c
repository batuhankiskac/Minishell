/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/11 19:34:19 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Forks a child process to handle heredoc input and waits for it.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @param pipe_fd An array where the pipe's file descriptors are stored.
 * @return The exit status of the child process.
 */
static int	fork_and_run_heredoc(t_shell *shell, t_redir *redir, int pipe_fd[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		print_error(NULL, "fork", strerror(errno), ERROR);
		return (ERROR);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_child_routine(shell, redir, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	return (status);
}

/**
 * @brief Manages the execution of a single here-document.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return 0 on success, ERROR on failure or interruption.
 */
static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int				pipe_fd[2];
	int				status;
	struct termios	saved_termios;

	redir->heredoc_fd = -1;
	tcgetattr(STDIN_FILENO, &saved_termios);
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));

	signal(SIGINT, SIG_IGN);
	status = fork_and_run_heredoc(shell, redir, pipe_fd);
	set_interactive_signals();
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_termios);

	if (WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		write(1, "\n", 1);
		return (ERROR);
	}
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Processes all here-documents in the command line before execution.
 * @param shell The main shell structure containing the command list.
 * @return 0 on success, ERROR if any heredoc is interrupted or fails.
 */
int	handle_heredoc_redir(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = shell->command;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (execute_single_heredoc(shell, redir) == ERROR)
					return (ERROR);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
