/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:45:09 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:45:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * token_reader - tokenizes input in shell->line into shell->tokens list.
 *
 * @shell: shell context containing line and tokens list head
 *
 * Returns 0 on success, or ERROR on failure.
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
			break;
		if (check_token(shell->line[pos]))
			pos += handle_token(shell->line, pos, &shell->tokens);
		else
			pos += read_words(pos, shell->line, &shell->tokens);
	}
	return (0);
}
