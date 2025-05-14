/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:32:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:32:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * skip_spaces - skips whitespace characters starting at index `start`.
 *
 * @str:   input string to scan
 * @start: index to begin skipping from
 *
 * Returns the number of whitespace characters skipped.
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
