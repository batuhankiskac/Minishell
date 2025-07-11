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
 * @brief The routine executed by the child process to read heredoc input.
 * @param shell The main shell structure for context (env, tokens, etc.).
 * @param redir The redirection structure containing the delimiter.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 * @details This function runs inside the forked child process. It reads user
 * input line by line. After each `readline` call, it checks if a signal was
 * caught. If so, it breaks the loop to proceed to the cleanup stage. After
 * the loop, it calls the centralized cleanup function `cleanup_child_and_exit`
 * with the appropriate exit code (130 for signal, 0 for success).
 */
static void	heredoc_child_routine(t_shell *shell, t_redir *redir,
	int pipe_write_fd)
{
	char	*line;
	char	*expanded_line;

	signal(SIGINT, heredoc_child_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (get_signal_flag() == SIGINT)
		{
			if (line)
				free(line);
			break ;
		}
		if (!line)
		{
			ft_printf(2, "minishell: warning: here-document at line %d "
				"delimited by end-of-file (wanted `%s')\n",
				shell->line_number, redir->file);
			break ;
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		if (redir->expand_content)
		{
			expanded_line = expand_string(line, shell->env, shell->exit_status);
			free(line);
			if (expanded_line)
			{
				ft_putendl_fd(expanded_line, pipe_write_fd);
				free(expanded_line);
			}
		}
		else
		{
			ft_putendl_fd(line, pipe_write_fd);
			free(line);
		}
	}
	if (get_signal_flag() == SIGINT)
		cleanup_child_and_exit(shell, NULL, NULL, 130);
	cleanup_child_and_exit(shell, NULL, NULL, 0);
}

/**
 * @brief Manages a here-document redirection using a dedicated child process.
 * @param shell A pointer to the main shell structure.
 * @param redir A pointer to the specific heredoc redirection being processed.
 * @param is_last_heredoc A flag to determine if this is the final heredoc
 * for a command, which dictates whether stdin should be redirected.
 * @return 0 on success, ERROR (-1) on failure or interruption.
 * @details This function implements the robust fork-pipe pattern for heredocs.
 * The parent process waits for the child to terminate and checks its exit
 * status to see if it was interrupted (exit code 130) or completed normally.
 * This isolates the main shell from signals during heredoc input.
 */
int	handle_heredoc_redir(t_shell *shell, t_redir *redir, int is_last_heredoc)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));
	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork", strerror(errno), ERROR));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_child_routine(shell, redir, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 130)
	{
		shell->heredoc_interrupted = 1;
		close(pipe_fd[0]);
		return (ERROR);
	}
	if (is_last_heredoc)
	{
		if (dup_fd(pipe_fd[0], STDIN_FILENO, "heredoc") == ERROR)
		{
			close(pipe_fd[0]);
			return (ERROR);
		}
	}
	close(pipe_fd[0]);
	return (0);
}
