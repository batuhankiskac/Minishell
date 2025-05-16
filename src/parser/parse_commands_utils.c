/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:00:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:55:03 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words_until_pipe(t_token *t)
{
	int	count;

	count = 0;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
			count++;
		t = t->next;
	}
	return (count);
}

int	init_command_args(t_command *cmd, int count)
{
	cmd->argc = count;
	cmd->args = safe_malloc(sizeof(char *) * (count + 1));
	return (cmd->args != NULL);
}

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
		t = t->next;
	}
	cmd->args[i] = NULL;
	*t_ptr = t;
}

void	set_command_name(t_command *cmd)
{
	if (cmd->args[0])
		cmd->cmd = cmd->args[0];
}
