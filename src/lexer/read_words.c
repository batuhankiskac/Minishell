/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_words.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:44:42 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:49:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * read_words - extracts a word or quoted sequence from str.
 *
 * @pos:    current index
 * @str:    input line
 * @tokens: address of token list to append to
 *
 * Returns number of characters consumed by the word.
 */
int	read_words(int pos, const char *str, t_token **tokens)
{
	int		start;
	char	quote;
	char	*word;

	start = pos;
	if (str[pos] == '\'' || str[pos] == '"')
	{
		quote = str[pos++];
		while (str[pos] && str[pos] != quote)
			pos++;
		if (str[pos] == quote)
			pos++;
	}
	else
	{
		while (str[pos] && !ft_isspace(str[pos])
			&& !check_token(str[pos]))
			pos++;
	}
	word = ft_substr(str, start, pos - start);
	token_add(tokens, word, pos - start);
	free(word);
	return (pos - start);
}
