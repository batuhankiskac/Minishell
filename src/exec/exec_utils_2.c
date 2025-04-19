/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:12:40 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/19 22:22:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* close_pipe_fd - close file descriptors for pipes and previous fd
*
* Closes the previous file descriptor and both ends of the pipe.
*
* Parameters:
*   int prev_fd       - previous file descriptor to close
*   int pipe_fd[2]    - array containing pipe file descriptors
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

/*
* run_command - execute the command in the shell context
*
* Checks if the command is a builtin or an external command and executes it.
* If the command is a pipeline, it calls execute_pipe to handle the pipeline.
*
* Parameters:
*   t_shell *shell    - shell context containing the command to execute
*/
void	run_command(t_shell *shell)
{
	if (!shell->command)
		return ;
	if (!shell->command->next)
	{
		if (is_builtin(shell->command->cmd))
			shell->exit_status = exec_builtin(shell);
		else
			shell->exit_status = exec_external(shell);
	}
	else
		shell->exit_status = execute_pipe(shell);
}
