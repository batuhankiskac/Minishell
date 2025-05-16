/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:54:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_commands(t_shell *shell)
{
	t_token		*t;
	t_command	*cmd;

	if (!shell->tokens)
		return (1);
	if (shell->tokens->type == TOKEN_PIPE)
		return (0);
	if (!validate_pipe_sequence(shell->tokens))
		return (0);
	t = shell->tokens;
	cmd = shell->command;
	while (cmd && t)
	{
		if (!process_command_block(cmd, &t))
			return (0);
		cmd = cmd->next;
	}
	return (1);
}
