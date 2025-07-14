/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 13:41:19 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands a heredoc line if required and writes it to the pipe.
 *
 * If expand_content is set, expands environment variables in the line before
 * writing. Otherwise, writes the line as is.
 *
 * @param line The line read from the user.
 * @param shell The main shell structure for context (env, exit_status).
 * @param redir The redirection structure containing expansion settings.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 */
static void	write_expanded_line(char *line, t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*expanded_line;

	if (redir->expand_content)
	{
		expanded_line = expand_string(line, shell->env, shell->exit_status);
		if (expanded_line)
		{
			ft_putendl_fd(expanded_line, pipe_write_fd);
			free(expanded_line);
		}
	}
	else
		ft_putendl_fd(line, pipe_write_fd);
	free(line);
}

/**
 * @brief Reads heredoc input in a loop within the main process.
 *
 * Reads lines from the user until the delimiter is found. Handles Ctrl+C (SIGINT)
 * and Ctrl+D (EOF) gracefully.
 *
 * @return 0 on success (delimiter found or EOF), ERROR on interruption (SIGINT).
 */
static int	heredoc_read_loop(t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*line;

	while (1)
	{
		reset_signal_flag();
		line = readline("> ");
		if (get_signal_flag() == SIGINT)
		{
			if (line)
				free(line);
			shell->exit_status = 130;
			return (ERROR);
		}
		if (!line)
		{
			ft_printf(2,
				"minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
				shell->line_number, redir->file);
			return (0);
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			return (0);
		}
		write_expanded_line(line, shell, redir, pipe_write_fd);
	}
}

/**
 * @brief Manages the execution of a single here-document without forking.
 *
 * Creates a pipe to store the heredoc content. Reads input from the user in the
 * main process. The read end of the pipe is stored in the redirection struct for
 * later use by execve.
 *
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return 0 on success, ERROR on failure or interruption.
 */
static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int	pipe_fd[2];
	int	read_status;

	redir->heredoc_fd = -1;
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));
	set_interactive_signals();
	read_status = heredoc_read_loop(shell, redir, pipe_fd[1]);
	close(pipe_fd[1]);
	if (read_status == ERROR)
	{
		close(pipe_fd[0]);
		return (ERROR);
	}
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Processes all here-documents in the command line before execution.
 *
 * Iterates through all commands and their redirections, calling
 * execute_single_heredoc for each heredoc found. If any heredoc is interrupted
 * by Ctrl+C, it stops processing and returns an error to prevent command execution.
 *
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
