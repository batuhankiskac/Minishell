/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:04:24 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:50:01 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * skip_pipe_tokens - advance past WORDs and a single PIPE token
 *
 * @t: current token pointer
 * Returns token after a PIPE (or NULL if end)
 */
static t_token	*skip_pipe_tokens(t_token *t)
{
	while (t && t->type != TOKEN_PIPE)
		t = t->next;
	if (t && t->type == TOKEN_PIPE)
		t = t->next;
	return (t);
}

/*
 * new_cmd - allocate and initialize a t_command node
 */
static t_command	*new_cmd(void)
{
	t_command	*c;

	c = safe_malloc(sizeof(*c));
	c->cmd = NULL;
	c->args = NULL;
	c->argc = 0;
	c->redir = NULL;
	c->next = NULL;
	return (c);
}

/*
 * build_command_list - split tokens into a linked list of commands
 *
 * @shell: shell context with shell->tokens populated
 * Returns 1 on success (at least one command), 0 otherwise
 */
int	build_command_list(t_shell *shell)
{
	t_token		*t;
	t_command	*head;
	t_command	*tail;

	head = NULL;
	tail = NULL;
	t = shell->tokens;
	while (t)
	{
		if (!head)
		{
			head = new_cmd();
			tail = head;
		}
		else
		{
			tail->next = new_cmd();
			tail = tail->next;
		}
		t = skip_pipe_tokens(t);
	}
	shell->command = head;
	return (head != NULL);
}
