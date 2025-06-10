/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 18:50:00 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles a token in the input string.
 *
 * This function checks the character at the specified position in the input
 * string. If it is a token character ('>' or '<'), it determines if it is a
 * single or double token (e.g., '>' or '>>', '<' or '<<') and adds it to the
 * tokens list.
 *
 * @param str The input string containing the command line.
 * @param pos The current position in the string where the token starts.
 * @param tokens A pointer to the list of tokens to which the new token will
 * be added.
 * @return The length of the token added (1 for single, 2 for double).
 */
int	handle_token(const char *str, int pos, t_token **tokens)
{
	int	len;

	len = 1;
	if ((str[pos] == '>' && str[pos + 1] == '>')
		|| (str[pos] == '<' && str[pos + 1] == '<'))
		len = 2;
	if (!token_add(tokens, str + pos, len))
		return (ERROR);
	return (len);
}
