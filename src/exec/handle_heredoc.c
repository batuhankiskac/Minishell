/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 18:51:54 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands a heredoc line if required and writes it to the pipe.
 *
 * This function handles variable expansion on the given line and writes the
 * result, followed by a newline, to the specified pipe.
 * @param line The line read from the user.
 * @param shell The main shell structure for context.
 * @param redir The redirection structure.
 * @param pipe_write_fd The file descriptor for the pipe's write-end.
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
 * This loop reads lines from stdin. If SIGINT is received, the global signal
 * flag is checked, and the loop continues, effectively restarting the prompt
 * on a new line. It terminates on EOF or when the delimiter is matched.
 * @return 0 on successful completion, ERROR if setup fails.
 */
static int	heredoc_read_loop(t_shell *shell, t_redir *redir, int pipe_write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_printf(2, "minishell: warning: heredoc delimited by EOF "
				"(wanted `%s')\n", redir->file);
			break ;
		}
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
 * This function sets up a temporary signal handler for SIGINT that allows
 * line-by-line interruption without exiting the heredoc. After the heredoc
 * input is complete, it restores the standard interactive signal handlers.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return 0 on success, ERROR on failure.
 */
static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int	pipe_fd[2];
	int	stdin_backup;

	redir->heredoc_fd = -1;
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe", strerror(errno), ERROR));
	stdin_backup = dup(STDIN_FILENO);
	heredoc_read_loop(shell, redir, pipe_fd[1]);
	close(pipe_fd[1]);
	redir->heredoc_fd = pipe_fd[0];
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
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
