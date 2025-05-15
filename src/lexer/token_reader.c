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

