/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:44:29 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:44:30 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * handle_token - processes metacharacter tokens: '|', '>>', '<', '<<'.
 *
 * @str:    input line
 * @pos:    current index in str
 * @tokens: address of token list to append to
 *
 * Returns number of characters consumed by the token.
 */
int	handle_token(const char *str, int pos, t_token **tokens)
{
	int	len;

	len = 1;
	if (str[pos] == '>' && str[pos + 1] == '>')
		len = 2;
	token_add(tokens, str + pos, len);
	return (len);
}
