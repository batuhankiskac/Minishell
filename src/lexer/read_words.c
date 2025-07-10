/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_words.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 23:26:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      A helper function to find the end of a quoted section.
 *
 * It starts right after an opening quote and scans until it finds the
 * matching closing quote, then returns the position after the closing quote.
 * @param pos  The starting position (the index of the opening quote).
 * @param str  The input string.
 * @return     The position after the closing quote, or ERROR if not found.
 */
static int	handle_quoted_section(int pos, const char *str)
{
	char	quote_char;

	quote_char = str[pos];
	pos++;
	while (str[pos] && str[pos] != quote_char)
	{
		pos++;
	}
	if (str[pos] != quote_char)
		return (ERROR);
	pos++;
	return (pos);
}

/**
 * @brief      Parses word boundaries handling quotes and special characters.
 *
 * This function determines where a word starts and ends in the input string.
 * It uses a helper function to manage quoted sections, which simplifies
 * the main loop and makes it more robust against parsing errors.
 * @param pos  The starting position in the input string.
 * @param str  The input string containing the command line.
 * @return     The ending position of the word, or ERROR if quote mismatch.
 */
static int	parse_word_boundaries(int pos, const char *str)
{
	int	new_pos;

	while (str[pos] && !ft_isspace(str[pos]) && !check_token(str[pos]))
	{
		if (str[pos] == '\'' || str[pos] == '\"')
		{
			new_pos = handle_quoted_section(pos, str);
			if (new_pos == ERROR)
				return (ERROR);
			pos = new_pos;
		}
		else
		{
			pos++;
		}
	}
	return (pos);
}

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
	int		end;
	char	*word;

	start = pos;
	end = parse_word_boundaries(pos, str);
	if (end == ERROR)
		return (ERROR);
	word = ft_substr(str, start, end - start);
	if (!word || !token_add(tokens, word, end - start))
	{
		free(word);
		return (ERROR);
	}
	free(word);
	return (end - start);
}
