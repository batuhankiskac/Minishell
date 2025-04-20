/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:54:31 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/19 22:21:20 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* setup_pipe_for_cmd - prepare a pipe for the given command
*
* Checks if cmd->next exists; if so, opens a new pipe and fills pipe_fd.
*
* Parameters:
*   t_command *cmd    - current command node
*   int pipe_fd[2]    - array to receive pipe file descriptors
*
* Returns:
*   0 on success, ERROR on failure
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

/*
* update_parent_pipe - update and close parent side of previous pipe
*
* Closes old prev_fd, assigns new read end for next command, closes write end.
*
* Parameters:
*   int *prev_fd      - pointer to previous read fd
*   int pipe_fd[2]    - current pipe fds
*   t_command *cmd    - current command node
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

/*
* execute_pipe_child - child process routine for piped commands
*
* Duplicates prev_fd to stdin, pipe_write_fd to stdout, sets up redirections,
* runs builtin or external command, then exits with its return code.
*
* Parameters:
*   t_shell *shell        - shell context with command info
*   int prev_fd           - fd for reading from previous pipe
*   int pipe_write_fd     - fd for writing to next pipe
*/
static void	execute_pipe_child(t_shell *shell, int prev_fd, int pipe_write_fd)
{
	int	ret;

	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 prev_fd");
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
	if (pipe_write_fd != -1)
	{
		if (dup_fd(pipe_write_fd, STDOUT_FILENO, "pipe") == ERROR)
			exit(EXIT_FAILURE);
	}
	if (setup_redir(shell) == ERROR)
		exit(EXIT_FAILURE);
	if (is_builtin(shell->command->cmd))
		ret = exec_builtin(shell);
	else
		ret = exec_external(shell);
	exit(ret);
}

/*
* handle_pipe_iteration - fork and execute one stage of a pipeline
*
* Sets up pipe, forks, in child calls execute_pipe_child, parent updates prev_fd.
*
* Parameters:
*   t_shell *shell        - shell context with command list
*   t_command *cmd        - current command node
*   int *prev_fd          - pointer to previous pipe read end
*
* Returns:
*   0 on success, ERROR on failure
*/
static int	handle_pipe_iteration(t_shell *shell, t_command *cmd, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		pipe_write_fd;

	if (setup_pipe_for_cmd(cmd, pipe_fd) == ERROR)
		return (ERROR);
	pid = fork();
	if (pid < 0)
		return (close_pipe_fd(*prev_fd, pipe_fd),
			perror("minishell: fork"), ERROR);
	if (pid == 0)
	{
		if (cmd->next != NULL)
			pipe_write_fd = pipe_fd[1];
		else
			pipe_write_fd = -1;
		execute_pipe_child(shell, *prev_fd, pipe_write_fd);
	}
	else
		update_parent_pipe(prev_fd, pipe_fd, cmd);
	return (0);
}

/*
* execute_pipe - execute a sequence of piped commands
*
* Iterates through commands linked list, handling each pipe iteration,
* then waits for all children and returns the exit status of the last.
*
* Parameters:
*   t_shell *shell    - shell context containing command pipeline
*
* Returns:
*   exit status of last command or ERROR
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
		ret = handle_pipe_iteration(shell, cmd, &prev_fd);
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
