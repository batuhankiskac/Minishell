/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:22:40 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 22:41:52 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Joins multiple heredoc lines into a single string.
 *
 * @param lines Array of lines to join
 * @param count Number of lines
 * @return Joined string or NULL on failure
 */
char	*join_heredoc(char **lines, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (count == 0)
		return (ft_strdup(""));
	result = ft_strdup(lines[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (i < count && result)
	{
		temp = ft_strjoin(result, "\n");
		free(result);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, lines[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

/**
 * @brief Frees an array of heredoc lines.
 *
 * @param lines Array of lines to free
 * @param count Number of lines
 */
void	free_heredoc(char **lines, int count)
{
	int	i;

	if (!lines)
		return ;
	i = -1;
	while (++i < count)
		free(lines[i]);
	free(lines);
}

/**
 * @brief Writes heredoc content to file for history.
 *
 * @param shell Shell structure containing line and redir
 * @param full_heredoc The complete heredoc content
 * @param eof_received Whether EOF was received (no delimiter)
 */
void	write_heredoc(t_shell *shell,
			char *full_heredoc, int eof_received)
{
	int	fd;

	fd = open_file("/tmp/minishell_heredoc_content",
			O_WRONLY | O_CREAT | O_TRUNC, 0644, "heredoc");
	if (fd != ERROR)
	{
		ft_putendl_fd(shell->line, fd);
		if (full_heredoc && ft_strlen(full_heredoc) > 0)
			ft_putendl_fd(full_heredoc, fd);
		if (!eof_received)
			ft_putstr_fd(shell->redir->file, fd);
		close(fd);
	}
}

/**
 * @brief Resizes the lines buffer when capacity is reached.
 *
 * @param lines Current lines array
 * @param capacity Current capacity
 * @return New capacity, or 0 on failure
 */
int	resize_lines_buffer(char ***lines, int capacity)
{
	char	**new_lines;
	int		new_capacity;
	int		i;

	new_capacity = capacity * 2;
	new_lines = malloc(sizeof(char *) * new_capacity);
	if (!new_lines)
		return (0);
	i = 0;
	while (i < capacity)
	{
		new_lines[i] = (*lines)[i];
		i++;
	}
	free(*lines);
	*lines = new_lines;
	return (new_capacity);
}
