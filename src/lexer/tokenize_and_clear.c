/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_and_clear.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:41:18 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/02 21:33:16 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes the next token from the line at the given position.
 * * Determines if the token is a special operator or a word, calls the
 * appropriate handler, and performs error checking.
 * @param shell The shell structure containing the token list.
 * @param line The input command line.
 * @param pos The current position in the line.
 * @return The number of characters read, or ERROR on failure.
 */
static int	process_next_token(t_shell *shell, char *line, int pos)
{
	int	result;

	if (check_token(line[pos]))
	{
		result = handle_token(line, pos, &shell->tokens);
		if (result == ERROR)
			return (clear_token_list(&shell->tokens), ERROR);
	}
	else
	{
		result = read_words(pos, line, &shell->tokens);
		if (result == ERROR)
			return (clear_token_list(&shell->tokens), ERROR);
	}
	return (result);
}

/**
 * @brief Tokenizes a line of input into a linked list of tokens.
 * * This function processes the input line character by character,
 * identifying tokens (words, pipes, redirections) and appending them
 * to the linked list of tokens in the shell structure.
 * @param line The input line to tokenize.
 * @param shell A pointer to the shell structure where tokens will be stored.
 * @return 0 on success, or ERROR if memory allocation fails.
 */
int	tokenize_line(char *line, t_shell *shell)
{
	int	pos;
	int	result;

	pos = 0;
	shell->tokens = NULL;
	while (line[pos])
	{
		while (line[pos] && ft_isspace(line[pos]))
			pos++;
		if (!line[pos])
			break ;

		result = process_next_token(shell, line, pos);
		if (result == ERROR)
			return (ERROR);
		pos += result;
	}
	return (0);
}

/**
 * @brief Clears the command list by freeing all allocated memory.
 *
 * This function iterates through the linked list of commands,
 * freeing each command's arguments and redirections, and then
 * freeing the command itself.
 *
 * @param cmd A pointer to the head of the command list to be cleared.
 */
void	clear_command_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		ft_free_all(cmd->args);
		free_redirections(cmd->redir);
		free(cmd);
		cmd = next;
	}
}

/**
 * @brief Clears the linked list of tokens by freeing all allocated memory.
 *
 * This function iterates through the linked list of tokens,
 * freeing each token's string and the token itself.
 *
 * @param tokens_head A pointer to the head of the token list to be cleared.
 */
void	clear_token_list(t_token **tokens_head)
{
	t_token	*current;
	t_token	*next;

	if (!tokens_head || !*tokens_head)
		return ;
	current = *tokens_head;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	*tokens_head = NULL;
}
