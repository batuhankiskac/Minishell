/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 18:34:04 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands a heredoc line if required and writes it to the pipe.
 *
 * If expand_content is set, expands environment variables in the line before
 * writing. Otherwise, writes the line as is. A newline is appended after
 * the content is written.
 * @param line The line read from the user (without newline).
 * @param shell The main shell structure for context.
 * @param redir The redirection structure containing expansion settings.
 * @param pipe_write_fd The file descriptor for the write-end of the pipe.
 */
static void	write_expanded_line(char *line, t_shell *shell, t_redir *redir,
		int pipe_write_fd)
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
 * @brief Reads heredoc input until the delimiter or EOF is found.
 *
 * This function reads from standard input. Since the controlling function
 * sets SIGINT to be ignored, this loop will not be interrupted by Ctrl+C.
 * It will only terminate upon reading the delimiter or encountering an EOF.
 * @return Always returns 0, indicating completion.
 */
static int	heredoc_read_loop(t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*line;
	size_t	len;

	while (1)
	{
		write(STDERR_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_printf(2, "minishell: warning: heredoc delimited by EOF "
				"(wanted `%s')\n", redir->file);
			break ;
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write_expanded_line(line, shell, redir, pipe_write_fd);
	}
	return (0);
}

/**
 * @brief Manages the execution of a single here-document.
 *
 * This function sets the disposition of SIGINT and SIGQUIT to SIG_IGN (ignore)
 * for the duration of the heredoc input. After the input is complete, it
 * restores the standard interactive signal handlers.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return Always returns 0, as interruptions are ignored.
 */
static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int	pipe_fd[2];

	redir->heredoc_fd = -1;
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	heredoc_read_loop(shell, redir, pipe_fd[1]);
	set_interactive_signals();
	close(pipe_fd[1]);
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Processes all here-documents in the command line before execution.
 *
 * Iterates through all commands and their redirections, calling
 * execute_single_heredoc for each heredoc found.
 * @param shell The main shell structure containing the command list.
 * @return 0 on success, ERROR if a pipe fails.
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
