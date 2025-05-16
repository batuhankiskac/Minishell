/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 20:25:16 by bkiskac          ###   ########.fr       */
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
	shell.line = NULL;
	shell.exit_status = 0;
	while (1)
	{
		reset_signal_flag();
		line = readline("minishell> ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			free(shell.line);
			clear_token_list(&shell.tokens);
			clear_command_list(shell.command);
			free_env(shell.env);
			rl_clear_history();
			break;
		}
		if (shell.line)
			free(shell.line);
		shell.line = ft_strdup(line);
		if (!shell.line)
		{
			perror("ft_strdup failed");
			free(line);
			continue;
		}
		if (*shell.line)
			add_history(shell.line);
		if (tokenize_line(shell.line, &shell) == ERROR
			|| !build_command_list(&shell)
			|| !parse_commands(&shell)
			|| !parse_redirections(&shell)
			|| !expander(&shell))
		{
			free(line);
			clear_command_list(shell.command);
			shell.command = NULL;
			clear_token_list(&shell.tokens);
			shell.tokens = NULL;
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
		clear_token_list(&shell.tokens);
		shell.tokens = NULL;
		free(line);
	}
	reset_signals();
	free(shell.line);
	free_env(shell.env);
	return (shell.exit_status);
}
