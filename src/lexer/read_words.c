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

