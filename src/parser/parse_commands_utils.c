/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:00:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/31 16:59:04 by bkiskac          ###   ########.fr       */
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
		// Bu kod bloğu rediraction hatasını çözmek için copilot tarafından eklendi.
		// Tekrardan bakıp kendin sorunu anlayıp düzeltirsen daha iyi olur.
		else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_HEREDOC)
		{
			if (t->next && t->next->type == TOKEN_WORD)
				t = t->next;
		}
		/**/
		t = t->next;
	}
	return (count);
}

/**
 * @brief Initializes the argument array for a command.
 *
 * This function allocates memory for the command's argument array based
 * on the specified count. The array is initialized to NULL.
 *
 * @param cmd A pointer to the command structure to initialize.
 * @param count The number of arguments to allocate space for.
 * @return 1 if the argument array is successfully initialized, 0 otherwise.
 */
int	init_command_args(t_command *cmd, int count)
{
	cmd->argc = count;
	cmd->args = safe_malloc(sizeof(char *) * (count + 1));
	return (cmd->args != NULL);
}

/**
 * @brief Populates the argument array for a command.
 *
 * This function iterates through the token list and copies word tokens
 * into the command's argument array. The token pointer is updated to
 * point to the next unprocessed token.
 *
 * @param t_ptr A pointer to the token pointer, which is updated as tokens
 *              are processed.
 * @param cmd A pointer to the command structure to populate.
 */
void	populate_args(t_token **t_ptr, t_command *cmd)
{
	int		i;
	t_token	*t;

	i = 0;
	t = *t_ptr;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
			cmd->args[i++] = ft_strdup(t->str);
		// Bu kod bloğu rediraction hatasını çözmek için copilot tarafından eklendi.
		// Tekrardan bakıp kendin sorunu anlayıp düzeltirsen daha iyi olur.
		else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_HEREDOC)
		{
			if (t->next && t->next->type == TOKEN_WORD)
				t = t->next;
		}
		/**/
		t = t->next;
	}
	cmd->args[i] = NULL;
	*t_ptr = t;
}

/**
 * @brief Sets the command name for a command structure.
 *
 * This function sets the command name to the first argument in the
 * argument array, if it exists.
 *
 * @param cmd A pointer to the command structure to update.
 */
void	set_command_name(t_command *cmd)
{
	if (cmd->args[0])
		cmd->cmd = cmd->args[0];
}
