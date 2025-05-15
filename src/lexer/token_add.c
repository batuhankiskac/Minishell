/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	token_add(t_token **tokens, const char *word, int len)
{
	t_token	*new;
	t_token	*cur;
	new = safe_malloc(sizeof(t_token));
	new->str = ft_strndup(word, len);
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

