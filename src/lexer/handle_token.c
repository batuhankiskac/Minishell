/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:39:27 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_token(const char *str, int pos, t_token **tokens)
{
	int	len;

	len = 1;
	if ((str[pos] == '>' && str[pos + 1] == '>')
		|| (str[pos] == '<' && str[pos + 1] == '<'))
		len = 2;
	token_add(tokens, str + pos, len);
	return (len);
}
