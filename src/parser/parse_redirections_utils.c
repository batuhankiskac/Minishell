/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:05:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 12:15:12 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(t_token_type tt)
{
	if (tt == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (tt == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (tt == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

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
