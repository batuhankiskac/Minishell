/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_block.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:45:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:26:58 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes a single command block from the token list.
 *
 * This function counts the number of words in the token list until a pipe
 * token is encountered, initializes the command's argument array, populates
 * the arguments, and sets the command name. If the token list contains a
 * pipe token, the pointer is advanced to the next token after the pipe.
 *
 * @param cmd A pointer to the command structure to populate.
 * @param t_ptr A pointer to the token pointer, which is updated as tokens
 *              are processed.
 * @return 1 if the command block is successfully processed, 0 otherwise.
 */
int	process_command_block(t_command *cmd, t_token **t_ptr)
{
	int	count;

	count = count_words_until_pipe(*t_ptr);
	if (count == 0)
		return (0);
	if (!init_command_args(cmd, count))
		return (0);
	populate_args(t_ptr, cmd);
	set_command_name(cmd);
	if (!cmd->cmd && cmd->argc > 0)
		cmd->cmd = cmd->args[0];
	else if (!cmd->cmd && count > 0)
		cmd->cmd = "";
	if (*t_ptr && (*t_ptr)->type == TOKEN_PIPE)
		*t_ptr = (*t_ptr)->next;
	return (1);
}
