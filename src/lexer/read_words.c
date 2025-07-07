/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_words.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 22:15:14 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads a word from the input string starting at the given position.
 *
 * This function reads a word from the input string, handling quotes and
 * spaces. It updates the position to the end of the word and adds the word
 * to the tokens list.
 *
 * @param pos The starting position in the input string.
 * @param str The input string containing the command line.
 * @param tokens A pointer to the list of tokens where the word will be added.
 * @return The length of the word read, or ERROR if an error occurs.
 */
int	read_words(int pos, const char *str, t_token **tokens)
{
	int		start;
	char	quote;
	char	*word;

	start = pos;
	while (str[pos] && !ft_isspace(str[pos]))
	{
		if (str[pos] == '\'' || str[pos] == '\"')
		{
			quote = str[pos++];
			while (str[pos] && str[pos] != quote)
				pos++;
			if (str[pos] != quote)
				return (ERROR);
			pos++;
		}
		else if (check_token(str[pos]))
			break ;
		else
			pos++;
	}
	word = ft_substr(str, start, pos - start);
	if (!word || !token_add(tokens, word, pos - start))
	{
		free(word);
		return (ERROR);
	}
	free(word);
	return (pos - start);
}
