/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:04:16 by bkiskac          ###   ########.fr       */
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
			return (perror("minishell: pipe"), ERROR);
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
static int	fork_pipeline_command(t_shell *shell, t_command *cmd, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (setup_pipe_for_cmd(cmd, pipe_fd) == ERROR)
		return (ERROR);
	pid = fork();
	if (pid < 0)
		return (close_pipe_fd(*prev_fd, pipe_fd),
			perror("minishell: fork"), ERROR);
	if (pid == 0)
		pipe_child_process(shell, cmd, *prev_fd, pipe_fd);
	else
		update_parent_pipe(prev_fd, pipe_fd, cmd);
	return (0);
}

/**
 * @brief Executes a pipeline of commands.
 *
 * This function iterates through a list of commands (`shell->command`)
 * and executes them as a pipeline. For each command, it calls
 * `handle_pipe_iteration` to set up pipes and fork a child process for
 * execution. The parent process waits for all child processes to complete
 * using a `while (wait(&status) > 0)` loop. If any step in
 * `handle_pipe_iteration` fails, it closes the `prev_fd` if it's open and
 * returns `ERROR`. Otherwise, it returns the exit status of the last command
 * in the pipeline.
 *
 * @param shell A pointer to the `t_shell` structure, containing the linked
 *              list of commands to be executed in the pipeline.
 * @return Returns the exit status of the last command in the pipeline (obtained
 *         via `WEXITSTATUS(status)`). Returns `ERROR` if any part of the
 *         pipeline setup or execution fails.
 */
int	execute_pipe(t_shell *shell)
{
	t_command	*cmd;
	int			prev_fd;
	int			status;
	int			ret;

	prev_fd = -1;
	cmd = shell->command;
	while (cmd)
	{
		ret = fork_pipeline_command(shell, cmd, &prev_fd);
		if (ret == ERROR)
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (ERROR);
		}
		cmd = cmd->next;
	}
	while (wait(&status) > 0)
		;
	return (WEXITSTATUS(status));
}
