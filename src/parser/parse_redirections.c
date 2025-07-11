/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 12:26:37 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Converts a token type to its corresponding redirection type.
 * @param tt The token type (e.g., TOKEN_REDIR_IN).
 * @return The corresponding t_redir_type enum value.
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
 * Allocates and initializes a new t_redir node with the given type and
 * filename. It duplicates the filename for storage.
 *
 * @param tt The token type of the redirection.
 * @param file The filename associated with the redirection.
 * @return A pointer to the newly created t_redir node, or NULL on failure.
 */
static t_redir	*new_redir_node(t_token_type tt, char *file)
{
	t_redir	*r;

	r = ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (print_error_null(NULL, NULL, strerror(errno)));
       r->type = get_redir_type(tt);
       r->here_fd = -1;
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

/**
 * @brief Appends a new redirection node to a command's redirection list.
 *
 * Creates a new redirection node and appends it to the end of the
 * command's redirection list.
 *
 * @param c The command to which the redirection is added.
 * @param t_ptr A pointer to the current token pointer, which will be advanced.
 * @param last A pointer to the last redirection node in the list.
 * @return 1 on success, 0 on failure.
 */
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

/**
 * @brief Parses all redirections for a single command block.
 *
 * Iterates through the tokens of a command block (until a pipe is found)
 * and creates redirection nodes for any redirection tokens encountered.
 *
 * @param c The command structure to populate with redirections.
 * @param t_ptr A pointer to the current token pointer.
 * @return 1 on success, 0 on failure.
 */
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

/**
 * @brief Parses redirections for all commands in the shell.
 *
 * Iterates through the command list and calls parse_cmd_redirs for each
 * command to handle its specific redirections.
 *
 * @param shell The main shell structure.
 * @return 1 on success, 0 on failure.
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
