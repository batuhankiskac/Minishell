/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_block.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:45:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 16:22:43 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if there are any redirection tokens until the next pipe.
 *
 * @param t A pointer to the current token in the token list.
 * @return 1 if redirection tokens are found, 0 otherwise.
 */
static int	has_redirection_tokens(t_token *t)
{
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_HEREDOC)
			return (1);
		t = t->next;
	}
	return (0);
}

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
	int	has_redirections;

	count = count_words_until_pipe(*t_ptr);
	has_redirections = has_redirection_tokens(*t_ptr);
	if (count == 0 && !has_redirections)
		return (0);
	if (count == 0 && has_redirections)
	{
		if (!init_command_args(cmd, 1))
			return (0);
		cmd->args[0] = ft_strdup("cat");
		cmd->args[1] = NULL;
		cmd->argc = 1;
		cmd->cmd = cmd->args[0];
	}
	else
	{
		if (!init_command_args(cmd, count))
			return (0);
		populate_args(t_ptr, cmd);
		set_command_name(cmd);
		if (!cmd->cmd && cmd->argc > 0)
			cmd->cmd = cmd->args[0];
		else if (!cmd->cmd && count > 0)
			cmd->cmd = "";
	}
	if (*t_ptr && (*t_ptr)->type == TOKEN_PIPE)
		*t_ptr = (*t_ptr)->next;
	return (1);
}
