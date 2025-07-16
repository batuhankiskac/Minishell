/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:15:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/16 18:26:43 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the write end of the pipe for the current command.
 * It closes the read end of the pipe as it is not needed in the child.
 * @param cmd A pointer to the current command in the pipeline.
 * @param pipe_fd An array containing the pipe's file descriptors.
 * @return The write end file descriptor if there is a next command, else -1.
 */
static int	init_pipe_write_fd(t_command *cmd, int pipe_fd[2])
{
	int	pipe_write_fd;

	if (cmd->next)
	{
		pipe_write_fd = pipe_fd[1];
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
	}
	else
		pipe_write_fd = -1;
	return (pipe_write_fd);
}

/**
 * @brief Sets up input/output redirection between pipes.
 * @param prev_fd The read end of the pipe from the previous command.
 * @param pipe_write_fd The write end of the pipe to the next command.
 * @param shell A pointer to the shell structure for error cleanup.
 */
static void	handle_pipe_redir(int prev_fd, int pipe_write_fd, t_shell *shell)
{
	if (prev_fd != -1)
	{
		if (dup_fd(prev_fd, STDIN_FILENO, "pipe_in") == ERROR)
			cleanup_child_and_exit(shell, NULL, NULL, 1);
	}
	if (pipe_write_fd != -1)
	{
		if (dup_fd(pipe_write_fd, STDOUT_FILENO, "pipe") == ERROR)
			cleanup_child_and_exit(shell, NULL, NULL, 1);
	}
}

/**
 * @brief Acts as a data pump for commands without an executable.
 * Reads from stdin and writes to stdout until EOF.
 * This is essential for handling redirections in pipelines like
 * `< file | cat`.
 * @param shell The shell structure for cleanup.
 * @param original_head The original head of the command list for cleanup.
 */
static void	handle_data_pump(t_shell *shell, t_command *original_head)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break ;
		if (write(STDOUT_FILENO, buffer, bytes_read) < 0)
			break ;
	}
	cleanup_child_and_exit(shell, NULL, original_head, 0);
}

/**
 * @brief Dispatches the command to the appropriate executor (built-in/external).
 * @param shell The shell structure.
 * @param original_head The original command list head for cleanup.
 */
static void	dispatch_command_execution(t_shell *shell, t_command *original_head)
{
	char	**env_array;
	int		exit_status;

	if (is_builtin(shell->command->cmd))
	{
		exit_status = exec_builtin(shell);
		cleanup_child_and_exit(shell, NULL, original_head, exit_status);
	}
	else
	{
		env_array = env_list_to_array(shell->env);
		if (!env_array)
			cleanup_child_and_exit(shell, NULL, original_head, 1);
		exec_external_direct(shell, env_array);
		cleanup_child_and_exit(shell, env_array, original_head, 1);
	}
}

/**
 * @brief Manages the execution of a command within a child process.
 * This is the main entry point for a child process in a pipeline.
 * It orchestrates pipe setup, redirections, and command execution.
 * @param shell A pointer to the `t_shell` structure.
 * @param cmd The specific `t_command` this child will execute.
 * @param prev_fd The read end of the pipe from the previous command.
 * @param pipe_fd The pipe for communication with the next child.
 */
void	pipe_child_process(t_shell *shell,
	t_command *cmd, int prev_fd, int pipe_fd[2])
{
	int			pipe_write_fd;
	t_command	*original_head;

	original_head = shell->command;
	shell->command = cmd;
	pipe_write_fd = init_pipe_write_fd(cmd, pipe_fd);
	handle_pipe_redir(prev_fd, pipe_write_fd, shell);
	reset_signals();
	if (setup_redir(shell) == ERROR)
		cleanup_child_and_exit(shell, NULL, original_head, 1);
	if (!shell->command->cmd)
		handle_data_pump(shell, original_head);
	else
		dispatch_command_execution(shell, original_head);
}
