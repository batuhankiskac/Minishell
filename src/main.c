/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:06:25 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 00:35:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_shell_loop(t_shell *shell)
{
	while (1)
	{
		init_signals();
		shell->line = readline("minishell> ");
		if (!shell->line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (shell->line && *shell->line)
		{
			add_history(shell->line);
			if (token_reader(shell) == ERROR)
			{
				free(shell->line);
				shell->tokens = NULL;
				continue ;
			}
			if (parse_commands(shell) == ERROR)
			{
				free(shell->line);
				shell->tokens = NULL;
				shell->command = NULL;
				continue ;
			}
			if (expander(shell) == ERROR)
			{
				free(shell->line);
				shell->tokens = NULL;
				shell->command = NULL;
				continue ;
			}
			run_command(shell);
			shell->tokens = NULL;
			shell->command = NULL;
		}
		free(shell->line);
		shell->line = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.line = NULL;
	shell.tokens = NULL;
	shell.command = NULL;
	shell.exit_status = 0;
	shell.env = env_init(envp);
	if (!shell.env)
	{
		ft_putstr_fd("Error: Environment initialization failed.\n", \
			STDERR_FILENO);
		return (1);
	}
	run_shell_loop(&shell);
	free_env(shell.env);
	return (shell.exit_status);
}
