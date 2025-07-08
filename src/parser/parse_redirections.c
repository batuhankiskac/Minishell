/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 11:41:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Maps token type to redirection type.
 *
 * @param tt The token type.
 * @return The corresponding redirection type.
 */
static t_redir_type	get_redir_type(t_token_type tt)
{
	if (tt == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (tt == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (tt == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

/**
 * @brief Creates a new redirection node.
 *
 * This function allocates memory for a new redirection node, sets its
 * type based on the token type, and assigns the specified file name.
 *
 * @param tt The token type representing the redirection type.
 * @param file The file name associated with the redirection.
 * @return A pointer to the newly created redirection node.
 */
static t_redir	*new_redir_node(t_token_type tt, char *file)
{
	t_redir	*r;

	r = malloc(sizeof(*r));
	if (!r)
		return (print_error_null(NULL, NULL, strerror(errno)));
	r->type = get_redir_type(tt);
	r->file = ft_strdup(file);
	r->original_file = ft_strdup(file);
	if (!r->file || !r->original_file)
	{
		if (r->file)
			free(r->file);
		if (r->original_file)
			free(r->original_file);
		free(r);
		return (print_error_null(NULL, NULL, strerror(errno)));
	}
	r->next = NULL;
	return (r);
}

/**
 * @brief Parses redirections for a single command.
 *
 * This function processes the token list to identify redirection tokens
 * and creates redirection nodes for the command. The token pointer is
 * updated as tokens are processed.
 *
 * @param c A pointer to the command structure to update.
 * @param t A pointer to the token pointer, which is updated as tokens
 *          are processed.
 * @return 1 if the redirections are successfully parsed, 0 otherwise.
 */
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

/**
 * @brief Parses redirections for all commands in the command list.
 *
 * This function iterates through the command list and parses redirections
 * for each command using `parse_cmd_redirs`. The token pointer is updated
 * as tokens are processed.
 *
 * @param shell A pointer to the shell structure containing the command list
 *              and token list.
 * @return 1 if all redirections are successfully parsed, 0 otherwise.
 */
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
