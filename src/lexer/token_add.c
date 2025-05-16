/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:40:52 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	token_add(t_token **tokens, const char *word, int len)
{
	t_token	*new_token;
	t_token	*current;

	new_token = safe_malloc(sizeof(t_token));
	new_token->str = ft_strndup(word, len);
	new_token->type = determine_token_type(word, len);
	new_token->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
