/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/11 12:28:44 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Opens a file with specified flags and prints an error on failure.
 * @param filename The path to the file to be opened.
 * @param flags Integer flags for `open()` (e.g., `O_RDONLY`, `O_WRONLY`).
 * @param mode The file mode (permissions) to be used if the file is created.
 * @return The file descriptor of the opened file on success, or `ERROR` on
 *         failure.
 */
int	open_file(char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		return (print_error(NULL, filename, strerror(errno), ERROR));
	return (fd);
}

/**
 * @brief Duplicates a file descriptor to another.
 *
 * This function uses `dup2()` to duplicate `old_fd` to `new_fd`. If `dup2()`
 * is successful, `old_fd` is closed. If `dup2()` fails, an error message is
 * printed to standard error, including a `type` string (e.g., "pipe",
 * "stdin") for context, using `perror`. `old_fd` is closed even on failure
 * (if it was a valid descriptor). The function returns 0 on success and
 * `ERROR` on failure.
 *
 * @param old_fd The file descriptor to be duplicated.
 * @param new_fd The target file descriptor for the duplication.
 * @param type A string describing the context of the duplication (e.g.,
 *             "pipe", "stdin"), used in error messages.
 * @return 0 on success, `ERROR` on failure.
 */
int	dup_fd(int old_fd, int new_fd, char *type)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		return (print_error("dup2", type, strerror(errno), ERROR));
	}
	close(old_fd);
	return (0);
}

/**
 * @brief Closes pipe-related file descriptors.
 *
 * This function is a utility to safely close file descriptors that might have
 * been used for pipes. It checks if `prev_fd` (the read end of a previous
 * pipe) is not -1 and closes it. It also checks if `pipe_fd[0]` (read end of
 * current pipe) and `pipe_fd[1]` (write end of current pipe) are not -1 and
 * closes them respectively. This is typically used for cleanup in error paths
 * or after pipe usage is complete.
 *
 * @param prev_fd The file descriptor for the read end of the previous pipe.
 *                A value of -1 indicates it's not open or not applicable.
 * @param pipe_fd An array of two integers representing the current pipe's file
 *                descriptors: `pipe_fd[0]` for read, `pipe_fd[1]` for write.
 *                A value of -1 for an element indicates it's not open.
 */
void	close_pipe_fd(int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1)
		close(prev_fd);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}

/**
 * @brief Checks if a given command is a built-in shell command.
 *
 * This function compares the input command string `cmd` against a predefined
 * list of built-in command names: "echo", "cd", "pwd", "export", "unset",
 * "env", and "exit". The comparison is done using `ft_strcmp`.
 *
 * @param cmd A string representing the command to be checked.
 * @return Returns 1 if `cmd` matches one of the built-in command names.
 *         Returns 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/**
 * @brief Initializes heredoc structure in shell.
 *
 * This function allocates and initializes the heredoc structure
 * within the shell, setting up initial values for capacity and count.
 *
 * @param shell The shell structure to initialize heredoc for.
 * @return 0 on success, ERROR on failure.
 */
int	init_heredoc(t_shell *shell)
{
	if (shell->heredoc)
		cleanup_heredoc(shell);
	shell->heredoc = malloc(sizeof(t_heredoc));
	if (!shell->heredoc)
		return (ERROR);
	shell->heredoc->pipe_fd = -1;
	shell->heredoc->eof_reached = 0;
	return (0);
}
