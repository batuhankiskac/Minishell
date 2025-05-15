/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:43:33 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 08:43:41 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * token_add — Yeni bir token oluşturup liste sonuna ekler.
 *
 * tokens   — Token listesinin başına işaret eden pointer’ın pointer’ı.
 * word     — Kaynak dize (henüz km içinde ft_substr ile ayıklanmış).
 * len      — word uzunluğu (ft_substr’dan dönen uzunluk).
 */
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
