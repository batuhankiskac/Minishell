/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:28:06 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)argc;
	(void)argv;
	init_signals();
	shell.env = env_init(envp);
	shell.command = NULL;
	shell.exit_status = 0;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		if (tokenize_line(line, &shell) == ERROR
		 || build_command_list(&shell) == ERROR
		 || parse_commands(&shell) == ERROR
		 || parse_redirections(&shell) == ERROR)
		{
			free(line);
			clear_command_list(shell.command);
			shell.command = NULL;
			continue ;
		}
		if (shell.command)
		{
			if (shell.command->next == NULL
			 && is_builtin(shell.command->cmd))
				shell.exit_status = exec_builtin(&shell);
			else
				shell.exit_status = exec_pipe(&shell);
		}
		clear_command_list(shell.command);
		shell.command = NULL;
		free(line);
	}
	reset_signals();
	free_env(shell.env);
	return (shell.exit_status);
}
