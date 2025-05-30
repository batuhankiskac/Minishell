/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:17:05 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads tokens from the input line and populates the shell's token list.
 *
 * This function iterates through the input line character by character,
 * skipping spaces and identifying tokens (words, pipes, redirections).
 * It uses helper functions to handle different types of tokens and
 * appends them to the shell's token list.
 *
 * @param shell A pointer to the `t_shell` structure containing the input line
 *              and the token list to be populated.
 * @return 0 on success, or an error code if an error occurs.
 */
int	token_reader(t_shell *shell)
{
	int	pos;
	int	skipped;

	pos = 0;
	shell->tokens = NULL;
	while (shell->line[pos])
	{
		skipped = skip_spaces(shell->line, pos);
		pos += skipped;
		if (!shell->line[pos])
			break ;
		if (check_token(shell->line[pos]))
			pos += handle_token(shell->line, pos, &shell->tokens);
		else
			pos += read_words(pos, shell->line, &shell->tokens);
	}
	return (0);
}
