/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 07:05:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the child process for heredoc input.
 *
 * This function is executed in the child process created for handling heredoc
 * input. It closes the read end of the pipe, sets up signal handlers for
 * SIGINT and SIGQUIT, and calls heredoc_read_loop to read input until the
 * delimiter is encountered. After reading is complete, it closes the write end
 * of the pipe and exits with the appropriate exit code. If a SIGINT signal was
 * received during input, it exits with code 130 (standard for interrupt).
 *
 * @param shell A pointer to the t_shell structure.
 * @param redir A pointer to t_redir structure containing heredoc information.
 * @param pipe_fd An array of two int representing the pipe file descriptors.
 */
static void	heredoc_child_process(t_shell *shell, t_redir *redir,
	int pipe_fd[2])
{
	int	exit_code;

	close(pipe_fd[0]);
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	reset_signal_flag();
	heredoc_read_loop(shell, redir, pipe_fd[1]);
	close(pipe_fd[1]);
	exit_code = 0;
	if (get_signal_flag() == SIGINT)
		exit_code = 130;
	cleanup_child_and_exit(shell, NULL, NULL, exit_code);
}

/**
 * @brief Handles the parent process for heredoc input.
 *
 * This function is executed in the parent process after forking for heredoc
 * handling. It ignores SIGINT signals, closes the write end of the pipe,
 * and waits for the child process to complete. If the child process exits
 * with status 130 (indicating it was interrupted by SIGINT), it closes the
 * read end of the pipe, sets heredoc_fd to -1, and returns 1 to indicate
 * interruption. Otherwise, it sets the heredoc_fd to the read end of the pipe
 * and returns 0 for normal completion.
 *
 * @param pid The process ID of the child process.
 * @param pipe_fd An array of two int representing the pipe file descriptors.
 * @param redir A pointer to the t_redir structure to store the heredoc fd.
 * @return Returns 1 if the heredoc was interrupted, 0 otherwise.
 */
static int	heredoc_parent_process(pid_t pid, int pipe_fd[2], t_redir *redir)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	set_interactive_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		redir->heredoc_fd = -1;
		write(1, "\n", 1);
		return (1);
	}
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Executes a single heredoc redirection.
 *
 * This function handles the execution of a single heredoc redirection by
 * creating a pipe and forking a child process. The child process reads input
 * until the delimiter is encountered, while the parent process waits for the
 * child to complete. If pipe creation or fork fails, appropriate error
 * messages are printed and an error code is returned. The heredoc_fd in the
 * redir structure is initialized to -1 and updated in the parent process if
 * successful.
 *
 * @param shell A pointer to the t_shell structure.
 * @param redir A pointer to the t_redir structure for the heredoc redirection.
 * @return Returns 0 on success, ERROR on pipe or fork failure, or 1
 * if interrupted.
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
 * @brief Helper to process heredocs for a single command's redirection list.
 *
 * This function iterates through a given redirection list, executes heredocs,
 * and cleans up non-final heredoc file descriptors immediately.
 *
 * @param shell The main shell structure.
 * @param redir_list The head of the redirection list to process.
 * @param last_heredoc A pointer to the last heredoc in the list.
 * @return 0 on success, 1 on interruption, or ERROR on failure.
 */
static int	process_redir_list(t_shell *shell, t_redir *redir_list,
	t_redir *last_heredoc)
{
	int	result;

	while (redir_list)
	{
		if (redir_list->type == REDIR_HEREDOC)
		{
			result = execute_single_heredoc(shell, redir_list);
			if (result != 0)
			{
				if (result == 1)
					shell->exit_status = 130;
				close_heredoc_pipes(shell);
				return (result);
			}
			if (redir_list != last_heredoc && redir_list->heredoc_fd > 2)
			{
				close(redir_list->heredoc_fd);
				redir_list->heredoc_fd = -1;
			}
		}
		redir_list = redir_list->next;
	}
	return (0);
}

/**
 * @brief Handles all heredoc redirections in the command list.
 *
 * This function iterates through all commands, finds the last heredoc for each,
 * and calls a helper function to process the redirections. If any processing
 * fails or is interrupted, it returns immediately with the appropriate status.
 *
 * @param shell A pointer to the t_shell structure containing the command list.
 * @return Returns 0 on success, ERROR on pipe or fork failure, or 1
 * if interrupted.
 */
int	handle_heredoc_redir(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*last_heredoc;
	int			result;

	cmd = shell->command;
	while (cmd)
	{
		last_heredoc = find_last_heredoc(cmd->redir);
		result = process_redir_list(shell, cmd->redir, last_heredoc);
		if (result != 0)
			return (result);
		cmd = cmd->next;
	}
	return (0);
}
