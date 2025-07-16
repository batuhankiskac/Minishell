/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/16 21:58:29 by bkiskac          ###   ########.fr       */
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
 * @brief Signal handler for heredoc process
 *
 * Shell değişkenine erişemediğimiz için burada cleanup_child_and_exit
 * kullanamıyoruz, sadece exit ile çıkış yapıyoruz. Child process'in
 * temizliği execute_single_heredoc içinde yapılıyor.
 */
static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	cleanup_child_and_exit(NULL, NULL, NULL, 130);
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
 * @brief Manages the execution of a single here-document using fork.
 *
 * This function forks a child process to handle heredoc input. The child
 * process sets up its own signal handler for SIGINT that exits cleanly,
 * while the parent waits and handles the child's exit status.
 * @param shell The main shell structure.
 * @param redir The heredoc redirection to process.
 * @return 0 on success, ERROR on failure, 1 if interrupted by SIGINT.
 */
static int	execute_single_heredoc(t_shell *shell, t_redir *redir)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

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
	{
		close(pipe_fd[0]);
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		heredoc_read_loop(shell, redir, pipe_fd[1]);
		close(pipe_fd[1]);
		cleanup_child_and_exit(shell, NULL, NULL, 0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		return (1);
	}
	redir->heredoc_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Processes all here-documents in the command line before execution.
 *
 * Iterates through all commands and their redirections, calling
 * execute_single_heredoc for each heredoc found.
 * @param shell The main shell structure containing the command list.
 * @return 0 on success, ERROR if a pipe fails, 1 if interrupted by SIGINT.
 */
int	handle_heredoc_redir(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;
	int			result;

	cmd = shell->command;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				result = execute_single_heredoc(shell, redir);
				if (result == ERROR)
					return (ERROR);
				if (result == 1)
				{
					shell->exit_status = 130;
					return (1);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
