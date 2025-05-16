/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:43:43 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*new_redir_node(t_token_type tt, char *file)
{
	t_redir			*r;
	t_redir_type	type;

	type = REDIR_HEREDOC;
	if (tt == TOKEN_REDIR_IN)
		type = REDIR_IN;
	else if (tt == TOKEN_REDIR_OUT)
		type = REDIR_OUT;
	else if (tt == TOKEN_REDIR_APPEND)
		type = REDIR_APPEND;
	r = safe_malloc(sizeof(*r));
	r->type = type;
	r->file = ft_strdup(file);
	r->next = NULL;
	return (r);
}

static int	parse_cmd_redirs(t_command *c, t_token **t)
{
	t_redir	*last;
	t_redir	*node;

	last = NULL;
	while (*t && (*t)->type != TOKEN_PIPE)
	{
		if ((*t)->type >= TOKEN_REDIR_IN && (*t)->type <= TOKEN_HEREDOC)
		{
			if (!(*t)->next || (*t)->next->type != TOKEN_WORD)
				return (0);
			node = new_redir_node((*t)->type, (*t)->next->str);
			if (!node)
				return (0);
			if (!c->redir)
				c->redir = node;
			else
				last->next = node;
			last = node;
			*t = (*t)->next->next;
			continue ;
		}
		*t = (*t)->next;
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
