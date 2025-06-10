/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 17:05:17 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Collects input lines for heredoc until delimiter is found.
 *
 * @param shell Shell structure containing redir
 * @param lines Array to store lines
 * @param pipe_fd Write end of pipe
 * @return Number of lines collected, or count if EOF received
 */
static int	collect_input_lines(t_shell *shell, char **lines, int pipe_fd)
{
	char	*line;
	int		count;

	count = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document at line 1", 2);
			ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(shell->redir->file, 2);
			ft_putstr_fd("')\n", 2);
			return (count | 0x80000000);
		}
		if (ft_strcmp(line, shell->redir->file) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipe_fd);
		lines[count++] = ft_strdup(line);
		free(line);
	}
	return (count);
}

/**
 * @brief Collects heredoc lines and handles history.
 *
 * @param shell Shell structure containing redir and original command line
 * @param pipe_fd Write end of pipe
 * @return 0 on success, ERROR on failure, 1 if EOF received
 */
static int	collect_heredoc_lines(t_shell *shell, int pipe_fd)
{
	char	**lines;
	char	*full_heredoc;
	int		count;
	int		eof_received;

	lines = malloc(sizeof(char *) * 1000);
	if (!lines)
		return (ERROR);
	count = collect_input_lines(shell, lines, pipe_fd);
	eof_received = (count & 0x80000000) != 0;
	count = count & 0x7FFFFFFF;
	full_heredoc = join_heredoc_lines(lines, count);
	write_heredoc_to_file(shell, full_heredoc, eof_received);
	free(full_heredoc);
	free_heredoc_lines(lines, count);
	if (eof_received)
	{
		shell->heredoc_eof = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles a here-document redirection (`<<`).
 *
 * This function manages the input for a here-document by creating a pipe,
 * collecting user input until delimiter is reached, and redirecting to stdin.
 * All heredoc lines are collected and added as a single history entry.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              redir structure with delimiter string in `shell->redir->file`.
 * @return Returns 0 on successful handling of the here-document and redirection
 *         of input. Returns `ERROR` if `pipe()` fails, or if `dup_fd()` fails.
 *         Returns 1 if EOF received (Ctrl+D).
 */
int	handle_heredoc_redir(t_shell *shell)
{
	int	pipe_fd[2];
	int	collect_result;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), ERROR);
	collect_result = collect_heredoc_lines(shell, pipe_fd[1]);
	if (collect_result == ERROR)
		return (close(pipe_fd[0]), close(pipe_fd[1]), ERROR);
	if (collect_result == 1)
		return (close(pipe_fd[0]), close(pipe_fd[1]), 1);
	close(pipe_fd[1]);
	if (dup_fd(pipe_fd[0], STDIN_FILENO, "heredoc") == ERROR)
		return (close(pipe_fd[0]), ERROR);
	close(pipe_fd[0]);
	return (0);
}
