/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int	count_words_until_pipe(t_token *t)
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

static int	init_command_args(t_command *cmd, int count)
{
	cmd->argc = count;
	cmd->args = safe_malloc(sizeof(char *) * (count + 1));
	return (cmd->args != NULL);
}

static void	populate_args(t_token **t_ptr, t_command *cmd)
{
	int		i;
	t_token	*t;
	t = *t_ptr;
	i = 0;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
			cmd->args[i++] = ft_strdup(t->str);
		t = t->next;
	}
	cmd->args[i] = NULL;
	*t_ptr = t;
}

static void	set_command_name(t_command *cmd)
{
	if (cmd->args[0])
		cmd->cmd = cmd->args[0];
}

int	parse_commands(t_shell *shell)
{
	t_token		*t;
	t_command	*cmd;
	int			count;
	t = shell->tokens;
	cmd = shell->command;
	while (cmd && t)
	{
		count = count_words_until_pipe(t);
		if (!init_command_args(cmd, count))
			return (0);
		populate_args(&t, cmd);
		set_command_name(cmd);
		if (t && t->type == TOKEN_PIPE)
			t = t->next;
		cmd = cmd->next;
	}
	return (1);
}

