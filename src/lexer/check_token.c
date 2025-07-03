/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/03 10:09:04 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is a token character.
 *
 * This function checks if the given character is one of the token characters
 * used in the shell syntax, specifically '|', '<', or '>'.
 *
 * @param c The character to check.
 * @return 1 if the character is a token, 0 otherwise.
 */
int	check_token(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == ';')
		return (1);
	return (0);
}
