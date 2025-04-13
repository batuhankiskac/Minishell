/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rediractions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:23:41 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 16:53:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int handle_input_redir(t_redir *redir)
{
	int fd;

	fd = open_file(redir->file, O_RDONLY, 0, "input");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDIN_FILENO, "input") == ERROR)
		return (ERROR);
	return (0);
}

static int handle_output_redir(t_redir *redir)
{
	int fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644, "output");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "output") == ERROR)
		return (ERROR);
	return (0);
}

static int handle_append_redir(t_redir *redir)
{
	int fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644, "append");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "append") == ERROR)
		return (ERROR);
	return (0);
}

int	setup_redir(t_shell *shell)
{
	t_redir	*redir;
	int		ret;

	redir = shell->command->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			ret = handle_input_redir(redir);
		else if (redir->type == REDIR_OUT)
			ret = handle_output_redir(redir);
		else if (redir->type == REDIR_APPEND)
			ret = handle_append_redir(redir);
		else if (redir->type == REDIR_HEREDOC)
			printf("heredoc redirection not implemented\n");
		else
			ret = 0;
		if (ret == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
