/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 12:15:07 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*new_redir_node(t_token_type tt, char *file)
{
	t_redir	*r;

	r = ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (print_error_null(NULL, NULL, strerror(errno)));
	r->type = get_redir_type(tt);
	r->original_file = ft_strdup(file);
	if (!r->original_file)
	{
		free(r);
		return (print_error_null(NULL, NULL, strerror(errno)));
	}
	if (set_redir_file(r, file) == -1)
	{
		free(r->original_file);
		free(r);
		return (print_error_null(NULL, NULL, strerror(errno)));
	}
	return (r);
}

static int	append_redir_node(t_command *c, t_token **t_ptr, t_redir **last)
{
	t_redir	*node;

	if (!(*t_ptr)->next || (*t_ptr)->next->type != TOKEN_WORD)
		return (0);
	node = new_redir_node((*t_ptr)->type, (*t_ptr)->next->str);
	if (!node)
		return (0);
	if (!*last)
		c->redir = node;
	else
		(*last)->next = node;
	*last = node;
	*t_ptr = (*t_ptr)->next->next;
	return (1);
}

static int	parse_cmd_redirs(t_command *c, t_token **t_ptr)
{
	t_redir	*last;

	last = c->redir;
	while (last && last->next)
		last = last->next;
	while (*t_ptr && (*t_ptr)->type != TOKEN_PIPE)
	{
		if ((*t_ptr)->type >= TOKEN_REDIR_IN
			&& (*t_ptr)->type <= TOKEN_HEREDOC)
		{
			if (!append_redir_node(c, t_ptr, &last))
				return (0);
			continue ;
		}
		*t_ptr = (*t_ptr)->next;
	}
	return (1);
}

int	parse_redirections(t_shell *shell)
{
	t_token		*t;
	t_command	*c;

	t = shell->tokens;
	c = shell->command;
	while (c && t)
	{
		if (!parse_cmd_redirs(c, &t))
			return (0);
		if (t && t->type == TOKEN_PIPE)
			t = t->next;
		c = c->next;
	}
	return (1);
}
