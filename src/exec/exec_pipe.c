/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/20 16:43:21 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets up a pipe for the current command in a pipeline.
 *
 * This function creates a pipe if the current command (`cmd`) has a subsequent
 * command (`cmd->next`). The file descriptors for the pipe are stored in
 * `pipe_fd`. If there is no next command, `pipe_fd[0]` and `pipe_fd[1]` are
 * set to -1, indicating no pipe is needed for the last command in the
 * pipeline. If the `pipe()` system call fails, an error message is printed
 * using `perror`, and the function returns `ERROR`.
 *
 * @param cmd A pointer to the current `t_command` structure in the pipeline.
 * @param pipe_fd An integer array of size 2, where the pipe's read and write
 *                file descriptors will be stored.
 * @return Returns 0 on successful pipe creation or if no pipe is needed.
 *         Returns `ERROR` if `pipe()` fails.
 */
static int	setup_pipe_for_cmd(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
			return (print_error(NULL, NULL, strerror(errno), ERROR));
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}

/**
 * @brief Updates pipe file descriptors in the parent process after forking.
 *
 * This function is called in the parent process after a child process has
 * been forked to handle a command in a pipeline. It closes the previous
 * pipe's read end (`*prev_fd`) if it was used. If the current command
 * (`cmd`) has a next command, it means a new pipe was created. In this case,
 * the parent closes the write end of the new pipe (`pipe_fd[1]`) and updates
 * `*prev_fd` to be the read end of the new pipe (`pipe_fd[0]`) for the next
 * iteration. If there is no next command, `*prev_fd` is set to -1.
 *
 * @param prev_fd A pointer to an integer storing the file descriptor of the
 *                read end of the pipe from the previous command. This value
 *                is updated by the function.
 * @param pipe_fd An integer array of size 2 containing the file descriptors
 *                for the current pipe (read end at `pipe_fd[0]`, write end
 *                at `pipe_fd[1]`).
 * @param cmd A pointer to the current `t_command` structure.
 */
static void	update_parent_pipe(int *prev_fd, int pipe_fd[2], t_command *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		*prev_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
	else
		*prev_fd = -1;
}

/**
 * @brief Handles a single iteration of command execution within a pipeline.
 *
 * This function manages the execution of one command in a pipeline. It first
 * sets up a pipe for the current command using `setup_pipe_for_cmd` if it's
 * not the last command. It then forks a child process. The child process
 * calls `pipe_child_process` to execute the command with appropriate pipe
 * redirections. The parent process calls `update_parent_pipe` to manage its
 * end of the pipe file descriptors. If `fork()` or `setup_pipe_for_cmd()`
 * fails, it closes any opened pipe file descriptors and returns `ERROR`.
 *
 * @param shell A pointer to the `t_shell` structure.
 * @param cmd A pointer to the current `t_command` to be executed.
 * @param prev_fd A pointer to an integer storing the file descriptor of the
 *                read end of the pipe from the previous command.
 * @return Returns 0 on success, `ERROR` on failure (e.g., fork or pipe error).
 */
static pid_t	fork_pipeline_command(t_shell *shell, t_command *cmd,
	int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (setup_pipe_for_cmd(cmd, pipe_fd) == ERROR)
		return (ERROR);
	pid = fork();
	if (pid < 0)
	{
		close_pipe_fd(*prev_fd, pipe_fd);
		return (print_error(NULL, NULL, strerror(errno), ERROR));
	}
	if (pid == 0)
		pipe_child_process(shell, cmd, *prev_fd, pipe_fd);
	else
		update_parent_pipe(prev_fd, pipe_fd, cmd);
	return (pid);
}

/**
 * @brief Waits for all child processes in a pipeline and returns the exit code.
 *
 * This function waits for all child processes to complete and returns the exit
 * code of the last command in the pipeline, matching bash's behavior.
 *
 * @param last_pid The PID of the last command in the pipeline.
 * @return The exit code of the last command in the pipeline.
 */
static int	wait_for_pipeline(pid_t last_pid)
{
	int		status;
	int		exit_code;
	pid_t	current_pid;

	exit_code = 0;
	while (1)
	{
		current_pid = wait(&status);
		if (current_pid == -1 && errno == EINTR)
			continue ;
		else if (current_pid <= 0)
			break ;
		if (current_pid == last_pid)
			exit_code = handle_wait_status(status, NULL);
	}
	return (exit_code);
}

/**
 * @brief Executes a pipeline of commands.
 *
 * This function iterates through a list of commands and executes them
 * as a pipeline. Crucially, after each child is forked, the parent
 * closes its copies of any heredoc file descriptors passed to that child.
 *
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns the exit status of the last command in the pipeline.
 */
int	execute_pipe(t_shell *shell)
{
	t_command	*cmd;
	int			prev_fd;
	pid_t		pid;
	pid_t		last_pid;

	prev_fd = -1;
	cmd = shell->command;
	last_pid = -1;
	while (cmd)
	{
		pid = fork_pipeline_command(shell, cmd, &prev_fd);
		if (pid == ERROR)
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (ERROR);
		}
		parent_close_heredoc_fds(cmd);
		if (!cmd->next)
			last_pid = pid;
		cmd = cmd->next;
	}
	return (wait_for_pipeline(last_pid));
}
