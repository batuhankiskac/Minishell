/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:11:52 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 18:55:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_heredoc_redir(t_redir *redir)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"),ERROR);
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
