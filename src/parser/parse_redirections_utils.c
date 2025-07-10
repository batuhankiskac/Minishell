/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:05:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 21:57:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets properties for a heredoc redirection.
 *
 * This function checks if the heredoc delimiter is quoted. If it is,
 * it disables content expansion and removes the quotes from the delimiter
 * string.
 * Otherwise, it sets the delimiter as is and enables expansion.
 *
 * @param r The redirection node to modify.
 * @param file The raw delimiter string from the token.
 * @return 0 on success, -1 on memory allocation failure.
 */
int	set_heredoc_properties(t_redir *r, const char *file)
{
	size_t	len;

	len = ft_strlen(file);
	r->expand_content = 1;
	if ((file[0] == '\'' && file[len - 1] == '\'')
		|| (file[0] == '"' && file[len - 1] == '"'))
	{
		r->expand_content = 0;
		r->file = ft_substr(file, 1, len - 2);
	}
	else
		r->file = ft_strdup(file);
	if (!r->file)
		return (-1);
	return (0);
}

/**
 * @brief Sets the file or delimiter for a redirection node.
 *
 * This function handles setting the `file` member of a `t_redir` struct.
 * If the redirection is a heredoc, it calls `set_heredoc_properties` to
 * handle special quoting rules. For other redirection types, it simply
 * duplicates the provided filename.
 *
 * @param r The redirection node to modify.
 * @param file The filename or delimiter string.
 * @return 0 on success, -1 on memory allocation failure.
 */
int	set_redir_file(t_redir *r, const char *file)
{
	if (r->type == REDIR_HEREDOC)
	{
		if (set_heredoc_properties(r, file) == -1)
			return (-1);
	}
	else
	{
		r->file = ft_strdup(file);
		if (!r->file)
			return (-1);
	}
	return (0);
}
