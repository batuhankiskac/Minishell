/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:26:14 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Skips tokens until the next pipe token is found.
 *
 * This function iterates through the token list starting from the given
 * token `t` and skips all tokens until a pipe token (`TOKEN_PIPE`) is
 * encountered. If a pipe token is found, it moves to the next token after
 * the pipe.
 *
 * @param t A pointer to the current token in the token list.
 * @return A pointer to the token after the pipe token, or NULL if the end
 *         of the token list is reached.
 */
static t_token	*skip_pipe_tokens(t_token *t)
{
	while (t && t->type != TOKEN_PIPE)
		t = t->next;
	if (t && t->type == TOKEN_PIPE)
		t = t->next;
	return (t);
}

/**
 * @brief Creates a new command node.
 *
 * This function allocates memory for a new `t_command` structure and
 * initializes its fields to default values (e.g., NULL for pointers,
 * 0 for integers). The `next` pointer is also set to NULL.
 *
 * @return A pointer to the newly created `t_command` structure.
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

/**
 * @brief Builds a linked list of commands from the token list.
 *
 * This function processes the token list in the shell structure and
 * creates a linked list of commands. Each command corresponds to a
 * segment of tokens separated by pipe tokens. The resulting command
 * list is stored in the `shell->command` field.
 *
 * @param shell A pointer to the shell structure containing the token list.
 * @return 1 if the command list is successfully built, 0 otherwise.
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
