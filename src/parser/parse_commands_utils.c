/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:00:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/24 11:40:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of words in the token list until a pipe token.
 *
 * This function iterates through the token list starting from the given
 * token and counts the number of word tokens until a pipe token is
 * encountered or the end of the list is reached.
 *
 * @param t A pointer to the current token in the token list.
 * @return The number of word tokens encountered.
 */
int	count_words_until_pipe(t_token *t)
{
	int	count;

	count = 0;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
			count++;
		else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_HEREDOC)
		{
			if (t->next && t->next->type == TOKEN_WORD)
				t = t->next;
		}
		t = t->next;
	}
	return (count);
}

/**
 * @brief Initializes the argument array for a command using calloc.
 *
 * This function now uses ft_calloc instead of malloc. This is a critical
 * safety improvement: calloc initializes the allocated memory to zero (NULL
 * for pointers), ensuring that the argument array is always NULL-terminated,
 * even before it's fully populated. This prevents errors in cleanup
 * functions like ft_free_all if parsing is interrupted by a memory error.
 *
 * @param cmd A pointer to the command structure to initialize.
 * @param count The number of arguments to allocate space for.
 * @return 1 if the argument array is successfully initialized, 0 otherwise.
 */
int	init_command_args(t_command *cmd, int count)
{
	cmd->argc = count;
	cmd->args = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->args)
	{
		ft_printf(2, "minishell: malloc error: %s\n", strerror(errno));
		return (0);
	}
	return (1);
}

/**
 * @brief Populates the argument array for a command.
 *
 * This function iterates through tokens and copies word tokens into the
 * command's argument array. It now returns an error code if any memory
 * allocation fails during the process, allowing the caller to perform
 * a proper cleanup.
 *
 * @param t_ptr A pointer to the token pointer.
 * @param cmd A pointer to the command structure to populate.
 * @return 1 on success, 0 on memory allocation failure.
 */
int	populate_args(t_token **t_ptr, t_command *cmd)
{
	int		i;
	t_token	*t;

	i = 0;
	t = *t_ptr;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
		{
			cmd->args[i] = ft_strdup(t->str);
			if (!cmd->args[i])
				return (0);
			i++;
		}
		else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_HEREDOC)
		{
			if (t->next && t->next->type == TOKEN_WORD)
				t = t->next;
		}
		t = t->next;
	}
	*t_ptr = t;
	return (1);
}
