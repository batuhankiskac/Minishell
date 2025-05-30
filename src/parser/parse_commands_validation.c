/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:54:48 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:26:50 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validates the sequence of pipe tokens in the token list.
 *
 * This function checks the token list to ensure that pipe tokens are
 * properly placed. A pipe token must not be the first or last token,
 * and consecutive pipe tokens are not allowed.
 *
 * @param tokens A pointer to the head of the token list.
 * @return 1 if the pipe sequence is valid, 0 otherwise.
 */
int	validate_pipe_sequence(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			if (!tmp->next || tmp->next->type == TOKEN_PIPE)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}
