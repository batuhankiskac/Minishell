/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_block.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:45:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 14:56:40 by bkiskac          ###   ########.fr       */
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
 * @brief Sets up a command with only redirections (no arguments).
 *
 * @param cmd A pointer to the command structure to populate.
 * @return 1 on success, 0 on failure.
 */
static int	setup_redirection_only_command(t_command *cmd)
{
	if (!init_command_args(cmd, 1))
		return (0);
	cmd->args[0] = ft_strdup("/bin/true");
	cmd->args[1] = NULL;
	cmd->argc = 1;
	cmd->cmd = cmd->args[0];
	return (1);
}

/**
 * @brief Sets up a regular command with arguments.
 *
 * @param cmd A pointer to the command structure to populate.
 * @param t_ptr A pointer to the token pointer.
 * @param count Number of arguments to allocate.
 * @return 1 on success, 0 on failure.
 */
static int	setup_regular_command(t_command *cmd, t_token **t_ptr, int count)
{
	if (!init_command_args(cmd, count))
		return (0);
	populate_args(t_ptr, cmd);
	set_command_name(cmd);
	if (!cmd->cmd && cmd->argc > 0)
		cmd->cmd = cmd->args[0];
	else if (!cmd->cmd && count > 0)
		cmd->cmd = "";
	return (1);
}

/**
 * @brief Processes a single command block from the token list.
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
		if (!setup_redirection_only_command(cmd))
			return (0);
	}
	else
	{
		if (!setup_regular_command(cmd, t_ptr, count))
			return (0);
	}
	if (*t_ptr && (*t_ptr)->type == TOKEN_PIPE)
		*t_ptr = (*t_ptr)->next;
	return (1);
}
