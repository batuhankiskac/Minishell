/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_and_clear.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:41:18 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:41:20 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize_line(char *line, t_shell *shell)
{
	int		i;
	t_token	*tokens;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (check_token(line[i]))
			i += handle_token(line, i, &tokens);
		else
			i += read_words(i, line, &tokens);
		if (!tokens)
			return (ERROR);
	}
	shell->tokens = tokens;
	return (0);
}

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
