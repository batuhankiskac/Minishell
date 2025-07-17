/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:02:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/17 18:19:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes a single line of input for a here-document.
 *
 * This function checks if the line is the delimiter. If not, it expands
 * variables (if required) and writes the resulting line to the pipe.
 *
 * @param line The line read from stdin.
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing the delimiter.
 * @param pipe_write_fd The file descriptor for the pipe's write-end.
 * @return 0 to continue the loop, 1 to break (delimiter found).
 */
static int	process_heredoc_line(char *line, t_shell *shell, t_redir *redir,
		int pipe_write_fd)
{
	char	*expanded_line;

	if (ft_strcmp(line, redir->file) == 0)
	{
		free(line);
		return (1);
	}
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
	return (0);
}

/**
 * @brief Reads input for a here-document until a delimiter is found.
 *
 * This function manages the loop for reading heredoc input. It now
 * distinguishes between an EOF (Ctrl+D) and an interruption (Ctrl+C)
 * by checking the global signal flag after `readline` returns NULL.
 *
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing the delimiter.
 * @param pipe_write_fd The file descriptor for the pipe's write-end.
 */
void	heredoc_read_loop(t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (get_signal_flag() == SIGINT)
				break ;
			ft_printf(2, "minishell: warning: heredoc delimited by EOF "
				"(wanted `%s')\n", redir->file);
			break ;
		}
		if (process_heredoc_line(line, shell, redir, pipe_write_fd))
			break ;
	}
}
