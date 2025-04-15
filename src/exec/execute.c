/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:54:31 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/15 20:44:25 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_pipe(t_shell *shell)
{
	t_command	*current_cmd;
	pid_t		pid;
	pid_t		last_pid;
	int			pipe_fd[2];
	int			prev_fd;

	current_cmd = shell->command;
	prev_fd = -1;
	last_pid = -1;
	while (current_cmd)
	{
		pid = create_pipe_and_fork(pipe_fd, current_cmd);
		if (pid < 0)
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (ERROR);
		}
		else if (pid == 0)
			child_process_routine(shell, current_cmd, prev_fd, pipe_fd);
		else
		{
			parent_process_routine(&prev_fd, pipe_fd, current_cmd);
			last_pid = pid;
		}
		current_cmd = current_cmd->next;
	}
	return (wait_for_children(last_pid));
}
