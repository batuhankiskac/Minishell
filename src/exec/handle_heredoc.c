/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 08:56:12 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Processes a single line of input during a heredoc session.
 * @param line The line read from the user.
 * @param shell A pointer to the shell structure.
 * @return     1 if delimiter is found, ERROR on failure, 0 otherwise.
 */
static int	process_heredoc_input_line(char *line, t_shell *shell)
{
	if (ft_strcmp(line, shell->redir->file) == 0)
	{
		free(line);
		return (1);
	}
	if (shell->heredoc->count >= shell->heredoc->capacity)
	{
		shell->heredoc->capacity = resize_lines_buffer(&shell->heredoc->lines,
				shell->heredoc->capacity);
		if (shell->heredoc->capacity == 0)
		{
			free(line);
			return (ERROR);
		}
	}
	ft_printf(shell->heredoc->pipe_fd, "%s\n", line);
	shell->heredoc->lines[shell->heredoc->count] = ft_strdup(line);
	if (!shell->heredoc->lines[shell->heredoc->count])
	{
		free(line);
		return (ERROR);
	}
	shell->heredoc->count++;
	free(line);
	return (0);
}

/**
 * @brief      The main input loop for a heredoc. Reads lines from the user
 * until the delimiter is entered or EOF is received.
 * @param shell    The shell structure.
 * @return         0 on success, ERROR on failure.
 */
static int	heredoc_input_loop(t_shell *shell)
{
	char	*line;
	int		ret;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			shell->heredoc->eof_received = 1;
			return (0);
		}
		shell->line_number++;
		ret = process_heredoc_input_line(line, shell);
		if (ret == 1)
			break ;
		if (ret == ERROR)
			return (ERROR);
	}
	return (0);
}

/**
 * @brief      Collects user input for a heredoc, processes it, and prepares
 * the history file.
 * @param shell   The shell structure.
 * @param show_warning A flag to indicate if EOF warnings should be shown.
 * @return        0 on success, ERROR on failure.
 */
static int	collect_heredoc(t_shell *shell, int show_warning)
{
	int	start_line_number;

	start_line_number = shell->line_number;
	shell->heredoc->lines = malloc(sizeof(char *) * shell->heredoc->capacity);
	if (!shell->heredoc->lines)
		return (ERROR);
	if (heredoc_input_loop(shell) == ERROR)
		return (ERROR);
	if (shell->heredoc->eof_received && show_warning)
		ft_printf(2, "minishell: warning: here-document at line %d "
			"delimited by end-of-file (wanted `%s')\n",
			start_line_number, shell->redir->file);
	write_heredoc(shell, shell->heredoc->lines,
		shell->heredoc->count, shell->heredoc->eof_received);
	if (shell->heredoc->eof_received)
		shell->heredoc_eof = 1;
	return (0);
}

/**
 * @brief      Sets up pipe and collects heredoc input.
 * @param shell A pointer to the t_shell structure.
 * @param is_last_heredoc Flag to determine if warning should be shown.
 * @param pipe_fd Array to store pipe file descriptors.
 * @return     0 on success, ERROR on failure.
 */
static int	setup_and_collect_heredoc(t_shell *shell, int is_last_heredoc,
	int pipe_fd[2])
{
	int	collect_result;

	if (init_heredoc(shell) == ERROR)
		return (ERROR);
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));
	shell->heredoc->pipe_fd = pipe_fd[1];
	shell->heredoc->capacity = 100;
	collect_result = collect_heredoc(shell, is_last_heredoc);
	close(pipe_fd[1]);
	shell->heredoc->pipe_fd = -1;
	if (collect_result == ERROR || shell->heredoc_eof)
	{
		close(pipe_fd[0]);
		if (shell->heredoc_eof)
			return (1);
		return (ERROR);
	}
	return (0);
}

/**
 * @brief      The main execution logic for any heredoc redirection.
 * This function replaces the old `handle_heredoc_redir` and
 * `handle_heredoc_collect_only`. It creates a pipe, collects input,
 * and optionally redirects stdin based on the `is_last_heredoc` flag.
 * @param shell A pointer to the t_shell structure.
 * @param is_last_heredoc Flag to determine if stdin should be redirected.
 * @return      0 on success, 1 on EOF, ERROR on failure.
 */
int	handle_heredoc_redir(t_shell *shell, int is_last_heredoc)
{
	int	pipe_fd[2];
	int	result;

	result = setup_and_collect_heredoc(shell, is_last_heredoc, pipe_fd);
	if (result != 0)
		return (result);
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
