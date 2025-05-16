/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 19:19:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *line;
	(void)argc;
	(void)argv;
	init_signals();
	shell.env = env_init(envp);
	shell.command = NULL;
	shell.tokens = NULL;
	shell.exit_status = 0;
	while (1)
	{
		 reset_signal_flag();
		 line = readline("minishell> ");
		 if (!line)
		 {
			 write(STDOUT_FILENO, "exit\n", 5);
			 break;
		 }
		 if (*line)
			 add_history(line);
		 if (tokenize_line(line, &shell) == ERROR
			 || !build_command_list(&shell)
			 || !parse_commands(&shell)
			 || !parse_redirections(&shell)
			 || !expander(&shell))
		{
			 free(line);
			 clear_command_list(shell.command);
			 shell.command = NULL;
			 continue;
		}
		if (shell.command)
		 {
			if (shell.command->next == NULL
				 && is_builtin(shell.command->cmd))
				shell.exit_status = exec_builtin(&shell);
			else
				shell.exit_status = execute_pipe(&shell);
		}
		clear_command_list(shell.command);
		shell.command = NULL;
		free(line);
	}
	reset_signals();
	free_env(shell.env);
	return (shell.exit_status);
}
