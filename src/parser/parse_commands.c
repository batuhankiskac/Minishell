/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/01 15:10:04 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses the token list into a linked list of commands.
 *
 * This function validates the token list for proper pipe placement,
 * processes each command block, and populates the command list in the
 * shell structure.
 *
 * @param shell A pointer to the shell structure containing the token list.
 * @return 1 if the commands are successfully parsed, 0 otherwise.
 */
int	parse_commands(t_shell *shell)
{
	t_token		*t;
	t_command	*cmd;

	if (!shell->tokens)
		return (1);
	if (shell->tokens->type == TOKEN_PIPE)
	{
		clear_command_list(shell->command);
		shell->command = NULL;
		return (0);
	}
	if (!validate_pipe_sequence(shell->tokens))
	{
		clear_command_list(shell->command);
		shell->command = NULL;
		return (0);
	}
	t = shell->tokens;
	cmd = shell->command;
	while (cmd && t)
	{
		if (!process_command_block(cmd, &t))
		{
			clear_command_list(shell->command);
			shell->command = NULL;
			return (0);
		}
		cmd = cmd->next;
	}
	return (1);
}
