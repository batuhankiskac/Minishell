/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:43:23 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:43:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * check_token - checks if a character is a metacharacter for tokens.
 *
 * @c: the character to test
 *
 * Returns 1 if c is '|', '<', or '>', else 0.
 */
int	check_token(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
