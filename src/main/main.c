/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:29:11 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	free(shell->line);
	clear_token_list(&shell->tokens);
	clear_command_list(shell->command);
	free_env(shell->env);
	rl_clear_history();
}

static void
	execute_commands(t_shell *shell)
{
	if (shell->command->next == NULL
		&& is_builtin(shell->command->cmd))
		shell->exit_status = exec_builtin(shell);
	else
		shell->exit_status = execute_pipe(shell);
}

static int	process_single_line(char *raw_line_ptr, t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(raw_line_ptr);
	if (!shell->line)
	{
		perror("ft_strdup failed");
		free(raw_line_ptr);
		return (1);
	}
	if (*shell->line)
		add_history(shell->line);
	if (tokenize_line(shell->line, shell) == ERROR
		|| !build_command_list(shell)
		|| !parse_commands(shell)
		|| !parse_redirections(shell)
		|| !expander(shell))
	{
		if (handle_command_parsing_error(raw_line_ptr, shell))
			return (1);
	}
	if (shell->command)
		execute_commands(shell);
	cleanup_current_loop_iteration(raw_line_ptr, shell);
	return (0);
}

static void	process_input_loop(t_shell *shell)
{
	char	*raw_line_ptr;

	while (1)
	{
		reset_signal_flag();
		raw_line_ptr = readline("minishell> ");
		if (!raw_line_ptr)
		{
			handle_eof(shell);
			break ;
		}
		if (process_single_line(raw_line_ptr, shell) == 1)
			continue ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_signals();
	shell.env = env_init(envp);
	shell.command = NULL;
	shell.tokens = NULL;
	shell.line = NULL;
	shell.exit_status = 0;
	process_input_loop(&shell);
	reset_signals();
	free(shell.line);
	free_env(shell.env);
	return (shell.exit_status);
}
