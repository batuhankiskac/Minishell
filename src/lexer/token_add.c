/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 18:52:16 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_add(t_token **tokens, const char *word, int len)
{
	t_token	*new;
	t_token	*cur;

	new = safe_malloc(sizeof(t_token));
	new->str = ft_strndup(word, len);
	// determine token type
	if (len == 1 && word[0] == '|')
		new->type = TOKEN_PIPE;
	else if (len == 1 && word[0] == '<')
		new->type = TOKEN_REDIR_IN;
	else if (len == 1 && word[0] == '>')
		new->type = TOKEN_REDIR_OUT;
	else if (len == 2 && word[0] == '>' && word[1] == '>')
		new->type = TOKEN_REDIR_APPEND;
	else if (len == 2 && word[0] == '<' && word[1] == '<')
		new->type = TOKEN_HEREDOC;
	else
		new->type = TOKEN_WORD;
	new->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new;
		return ;
	}
	cur = *tokens;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

