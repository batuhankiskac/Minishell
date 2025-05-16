/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_block.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:45:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:54:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
