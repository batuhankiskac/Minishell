/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:40:34 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/12 17:15:43 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands a heredoc line if required and writes it to the pipe.
 * @param line The line read from the user.
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing expansion settings.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 */
static void	write_heredoc_line(char *line, t_shell *shell, t_redir *redir,
	int pipe_write_fd)
{
	char	*expanded_line;

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

/**
 * @brief Processes a single line of input for a here-document.
 * @param line The line read from the user.
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing the delimiter.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 * @return Returns 1 to continue the loop, 0 to break.
 */
int	process_heredoc_line(char *line, t_shell *shell, t_redir *redir,
	int pipe_write_fd)
{
	char	*trimmed_line;

	if (get_signal_flag() == SIGINT || !line)
	{
		if (!line && get_signal_flag() != SIGINT)
			ft_printf(2, "minishell: warning: here-document at line %d "
				"delimited by end-of-file (wanted `%s')\n",
				shell->line_number, redir->file);
		if (line)
			free(line);
		return (0);
	}
	if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
		trimmed_line = ft_substr(line, 0, ft_strlen(line) - 1);
	else
		trimmed_line = ft_strdup(line);
	free(line);
	if (ft_strcmp(trimmed_line, redir->file) == 0)
	{
		free(trimmed_line);
		return (0);
	}
	write_heredoc_line(trimmed_line, shell, redir, pipe_write_fd);
	return (1);
}

/**
 * @brief The main routine executed by the child process to read heredoc input.
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing the delimiter.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 */
void	heredoc_child_routine(t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*line;

	init_heredoc_signals();
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!process_heredoc_line(line, shell, redir, pipe_write_fd))
			break ;
	}
	if (get_signal_flag() == SIGINT)
		cleanup_child_and_exit(shell, NULL, NULL, 130);
	cleanup_child_and_exit(shell, NULL, NULL, 0);
}
