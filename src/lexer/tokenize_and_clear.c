/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_and_clear.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created by GitHub Copilot                          +#+  +:+       +#+        */
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
