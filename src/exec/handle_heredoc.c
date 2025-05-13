/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:11:52 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/14 23:16:36 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* handle_heredoc_redir - Handles heredoc redirection.
*
* This function creates a pipe and reads lines from the user
* until the specified delimiter is encountered. The lines are
* written to the pipe, which is then used as input for the command.
*
* Parameters:
*   t_redir *redir - A pointer to the redirection structure
* containing the heredoc delimiter.
*
* Returns:
*   0 on success, ERROR on failure.
*/
int	handle_heredoc_redir(t_redir *redir)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (dup_fd(pipe_fd[0], STDIN_FILENO, "heredoc") == ERROR)
		return (close(pipe_fd[0]), ERROR);
	close(pipe_fd[0]);
	return (0);
}
