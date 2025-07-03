/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/03 11:59:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Determines the type of token based on the word and its length.
 *
 * This function checks the first character(s) of the word to determine
 * if it is a pipe, input redirection, output redirection, append redirection,
 * heredoc, or a regular word.
 *
 * @param word The string to analyze.
 * @param len The length of the string.
 * @return The type of token as defined in `t_token_type`.
 */
static t_token_type	determine_token_type(const char *word, int len)
{
	if (len == 1 && word[0] == '|')
		return (TOKEN_PIPE);
	else if (len == 1 && word[0] == '<')
		return (TOKEN_REDIR_IN);
	else if (len == 1 && word[0] == '>')
		return (TOKEN_REDIR_OUT);
	else if (len == 2 && word[0] == '>' && word[1] == '>')
		return (TOKEN_REDIR_APPEND);
	else if (len == 2 && word[0] == '<' && word[1] == '<')
		return (TOKEN_HEREDOC);
	else
		return (TOKEN_WORD);
}

/**
 * @brief Adds a new token to the end of the linked list of tokens.
 *
 * This function creates a new token with the specified word and its length,
 * determines its type, and appends it to the end of the linked list of tokens.
 *
 * @param tokens A pointer to the head of the linked list of tokens.
 * @param word The string to be stored in the new token.
 * @param len The length of the string to be stored in the new token.
 * @return 1 on success, 0 on failure.
 */
int	token_add(t_token **tokens, const char *word, int len)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("malloc error"), 0);
	new_token->str = ft_strndup(word, len);
	if (!new_token->str)
	{
		free(new_token);
		return (perror("ft_strndup error"), 0);
	}
	new_token->type = determine_token_type(word, len);
	new_token->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_token;
		return (1);
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
	return (1);
}
