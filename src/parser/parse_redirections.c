/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:22:37 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:50:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * new_redir_node - allocate and initialize a t_redir node
 */
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

/*
 * parse_cmd_redirs - extract redirections for one command
 *
 * @c:   current command node
 * @t:   address of current token pointer
 */
static void	parse_cmd_redirs(t_command *c, t_token **t)
{
	t_redir	*last;
	t_redir	*node;

	last = NULL;
	while (*t && (*t)->type != TOKEN_PIPE)
	{
		if ((*t)->type >= TOKEN_REDIR_IN
			&& (*t)->type <= TOKEN_HEREDOC
			&& (*t)->next)
		{
			node = new_redir_node((*t)->type, (*t)->next->str);
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
}

/*
 * parse_redirections - populate each command’s redir list
 */
int	parse_redirections(t_shell *shell)
{
	t_token		*t;
	t_command	*c;

	t = shell->tokens;
	c = shell->command;
	while (c && t)
	{
		parse_cmd_redirs(c, &t);
		if (t && t->type == TOKEN_PIPE)
			t = t->next;
		c = c->next;
	}
	return (1);
}
