/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:23:41 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/14 23:16:22 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// < input redirection
static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_RDONLY, 0, "input");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDIN_FILENO, "input") == ERROR)
		return (ERROR);
	return (0);
}

// > output redirection
static int	handle_output_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644, "output");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "output") == ERROR)
		return (ERROR);
	return (0);
}

// >> append redirection
static int	handle_append_redir(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644, "append");
	if (fd == ERROR)
		return (ERROR);
	if (dup_fd(fd, STDOUT_FILENO, "append") == ERROR)
		return (ERROR);
	return (0);
}

/*
* setup_redir - Sets up redirections for the command.
*
* This function iterates through the redirection list in the shell
* structure and handles each type of redirection (input, output,
* append, heredoc) by calling the appropriate handler function.
*
* Parameters:
*   t_shell *shell - A pointer to the shell structure containing
*                   the command and redirection information.
* Returns:
*   0 on success, ERROR on failure.
*/
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
			ret = handle_heredoc_redir(redir);
		else
			ret = 0;
		if (ret == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (0);
}
