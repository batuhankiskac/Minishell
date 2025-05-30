/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:15:49 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Skips spaces, tabs, and newlines in a string starting from a given
 * position.
 *
 * This function counts the number of consecutive whitespace characters
 * (spaces, tabs, and newlines) starting from the specified index in the string.
 * It returns the count of these characters.
 *
 * @param str The input string to check for whitespace.
 * @param start The starting index in the string to begin checking.
 * @return The number of whitespace characters found starting from `start`.
 */
int	skip_spaces(const char *str, int start)
{
	int	count;

	count = 0;
	while (str[start + count]
		&& (str[start + count] == ' '
			|| str[start + count] == '\t'
			|| str[start + count] == '\n'))
		count++;
	return (count);
}
