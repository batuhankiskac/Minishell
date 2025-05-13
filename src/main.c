/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:06:25 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/13 21:28:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_minishell(t_tools *tools, char **envp)
{
	parse_envp(tools);
	find_pwd(tools);
	tools->envp = envp;
}

void	signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_tools	tools;
	char	*line;

	(void)argc;
	(void)argv;
	init_minishell(&tools, envp);
	signal_handlers();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		if (!ft_strncmp(line, "exit", 4) && (!line[4] || line[4] == ' '))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}
