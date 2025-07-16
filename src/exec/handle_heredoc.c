/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/16 23:02:41 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief The main logic for the child process created to handle a heredoc.
 *
 * This function is executed after a fork. It sets up signal handlers for
 * the child, calls the read loop to get input, and then cleans up and exits.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection being processed.
 * @param pipe_fd The file descriptors for the pipe.
 */
static void	heredoc_child_process(t_shell *shell, t_redir *redir,
	int pipe_fd[2])
{
	close(pipe_fd[0]);
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	heredoc_read_loop(shell, redir, pipe_fd[1]);
	close(pipe_fd[1]);
	cleanup_child_and_exit(shell, NULL, NULL, 0);
}

/**
 * @brief The main logic for the parent process after forking for a heredoc.
 *
 * This function waits for the child process to complete, checks its exit
 * status for interruptions (Ctrl+C), and sets up the heredoc file descriptor.
 * @param pid The process ID of the child.
 * @param pipe_fd The file descriptors for the pipe.
 * @param redir The heredoc redirection being processed.
 * @return 0 on success, 1 if the child was interrupted by SIGINT.
 */
static int	heredoc_parent_process(pid_t pid, int pipe_fd[2], t_redir *redir)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		return (1);
	}
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Manages a single here-document by forking and delegating tasks.
 *
 * This function orchestrates the creation of a heredoc. It creates a pipe,
 * forks, and then calls the appropriate child or parent handler function.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return 0 on success, ERROR on pipe/fork failure, 1 if interrupted.
 */
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

/**
 * @brief Processes all here-documents in the command line before execution.
 *
 * This function serves as the main entry point for heredoc processing. It
 * iterates through all commands and their redirections, calling
 * `execute_single_heredoc` for each heredoc found.
 * @param shell The main shell structure containing the command list.
 * @return 0 on success, ERROR if a pipe/fork fails, 1 if interrupted.
 */
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
