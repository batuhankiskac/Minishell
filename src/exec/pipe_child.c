/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:15:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 08:33:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the write end of the pipe for the current command.
 * @param cmd A pointer to the current command in the pipeline.
 * @param pipe_fd An array containing the read and write file descriptors of
 *                the pipe.
 * @return Returns the write end file descriptor of the pipe if there is a
 * next command, otherwise returns -1.
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
 * @brief Handles both input and output pipe redirection in a child process.
 *
 * This function sets up pipe redirection for both input and output:
 * - If `prev_fd` is valid (not -1), it redirects the previous command's output
 *   to this process's stdin using `dup2`.
 * - If `pipe_write_fd` is valid (not -1), it redirects this process's stdout
 *   to the next command's input using `dup_fd`.
 * If any redirection fails, the child process exits with `EXIT_FAILURE`.
 *
 * @param prev_fd The read end of the pipe from the previous command.
 *                A value of -1 indicates no input pipe (first command).
 * @param pipe_write_fd The write end of the pipe to the next command.
 *                      A value of -1 indicates no output pipe (last command).
 * @param shell A pointer to the shell structure for cleanup on error.
 */
static void	handle_pipe_redir(int prev_fd, int pipe_write_fd, t_shell *shell)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 prev_fd");
			cleanup_child_process(shell, NULL);
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
	if (pipe_write_fd != -1)
	{
		if (dup_fd(pipe_write_fd, STDOUT_FILENO, "pipe") == ERROR)
		{
			cleanup_child_process(shell, NULL);
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * @brief Sets up redirections and executes the command in the child process.
 *
 * This function is called within the child process to execute its assigned
 * command.
 * 1. It first calls `setup_redir(shell)` to handle any file redirections
 *    (e.g., `<`, `>`, `>>`). If `setup_redir` fails, the child exits with
 *    `EXIT_FAILURE`.
 * 2. It then checks if the command is a built-in using `is_builtin()`.
 *    - If it is a built-in, `exec_builtin()` is called, and the child exits
 *      with the status returned by the built-in.
 *    - If it is not a built-in (i.e., an external command),
 * 		`exec_external_direct()`
 *      is called, and the child exits with the status returned by the external
 *      command execution (which internally calls `execve` that doesn't return
 *      on success, or exits on `execve` failure).
 *
 * @param shell A pointer to the `t_shell` structure, containing the command
 *              and its context.
 */
static void	execute_child_cmd(t_shell *shell)
{
	char	**env_array;
	int		exit_status;

	reset_signals();
	if (setup_redir(shell) == ERROR)
	{
		cleanup_child_process(shell, NULL);
		exit(EXIT_FAILURE);
	}
	if (is_builtin(shell->command->cmd))
	{
		exit_status = exec_builtin(shell);
		cleanup_child_process(shell, NULL);
		exit(exit_status);
	}
	env_array = env_list_to_array(shell->env);
	if (!env_array)
	{
		cleanup_child_process(shell, NULL);
		exit(EXIT_FAILURE);
	}
	exec_external_direct(shell, env_array);
	cleanup_child_process(shell, env_array);
	exit(EXIT_FAILURE);
}

/**
 * @brief Manages the execution of a command within a child process in a
 * pipeline.
 *
 * This function is the main entry point for a child process created to execute
 * a command as part of a pipeline. It performs the following steps:
 * 1. Sets `shell->command` to the specific `cmd` this child is responsible for.
 * 2. Initializes the pipe's write end file descriptor using
 * `init_pipe_write_fd`,
 *    determining if output should go to a pipe for the next command.
 * 3. Sets up input redirection using `handle_input_redirection`, taking input
 *    from `prev_fd` (the output of the previous command's pipe).
 * 4. Sets up output redirection using `handle_output_redirection`, sending
 * output to `pipe_write_fd` (the input of the next command's pipe).
 * 5. Calls `execute_child_cmd` to handle file redirections and then execute
 *    either a built-in or an external command. This function will ultimately
 *    cause the child process to exit.
 *
 * @param shell A pointer to the `t_shell` structure.
 * @param cmd A pointer to the specific `t_command` this child process will
 * execute.
 * @param prev_fd The file descriptor for the read end of the pipe from the
 *                previous command. For the first command, this is typically -1.
 * @param pipe_fd An array containing the read (`pipe_fd[0]`) and write
 *                (`pipe_fd[1]`) file descriptors of the pipe created by the
 *                parent for communication *from* this child *to* the next child.
 *                `pipe_fd[0]` is closed by `init_pipe_write_fd` if `cmd->next`.
 */
void	pipe_child_process(t_shell *shell,
	t_command *cmd, int prev_fd, int pipe_fd[2])
{
	int	pipe_write_fd;

	shell->command = cmd;
	pipe_write_fd = init_pipe_write_fd(cmd, pipe_fd);
	handle_pipe_redir(prev_fd, pipe_write_fd, shell);
	execute_child_cmd(shell);
}
