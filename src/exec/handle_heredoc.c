/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:04:36 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles a here-document redirection (`<<`).
 *
 * This function manages the input for a here-document. It creates a pipe.
 * It then enters a loop, reading lines of input from the user using `readline("> ")`.
 * Each line read is compared with the delimiter specified in `redir->file`.
 * - If the line is `NULL` (EOF) or matches the delimiter, the loop breaks.
 * - Otherwise, the line is written to the write-end of the pipe, followed by a
 *   newline character.
 *
 * After the loop, the write-end of the pipe (`pipe_fd[1]`) is closed.
 * The read-end of the pipe (`pipe_fd[0]`) is then duplicated to `STDIN_FILENO`
 * using `dup_fd`. If this duplication fails, an error is returned. Finally,
 * the original read-end of the pipe (`pipe_fd[0]`) is closed.
 *
 * This effectively makes the content entered by the user (up to the delimiter)
 * available as standard input for the command associated with this redirection.
 *
 * @param redir A pointer to the `t_redir` structure, which contains the
 *              delimiter string in `redir->file` for the here-document.
 * @return Returns 0 on successful handling of the here-document and redirection
 *         of input. Returns `ERROR` if `pipe()` fails, or if `dup_fd()` fails.
 */
int	handle_heredoc_redir(t_redir *redir)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (dup_fd(pipe_fd[0], STDIN_FILENO, "heredoc") == ERROR)
		return (close(pipe_fd[0]), ERROR);
	close(pipe_fd[0]);
	return (0);
}
