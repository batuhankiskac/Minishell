/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/17 18:27:11 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief The main interactive loop of the shell.
 *
 * This function is the heart of the shell's interactive mode. It continuously
 * displays a prompt, reads a line of input from the user, and processes it.
 * The loop terminates when `readline` returns NULL, which typically happens
 * when the user presses Ctrl-D on an empty line (EOF). It also handles the
 * `SIGINT` signal (Ctrl-C) by resetting the prompt on a new line.
 *
 * @param shell A pointer to the `t_shell` structure, which is passed to the
 *              processing functions.
 */
static void	input_loop(t_shell *shell)
{
	char	*raw_line_ptr;
	int		result;

	while (1)
	{
		raw_line_ptr = readline("minishell> ");
		if (get_signal_flag() == SIGINT)
			shell->exit_status = 130;
		reset_signal_flag();
		if (!raw_line_ptr)
		{
			if (isatty(0))
				ft_printf(1, "exit\n");
			break ;
		}
		if (*raw_line_ptr)
		{
			result = process_line(raw_line_ptr, shell);
			if (result)
				continue ;
		}
		else
			free(raw_line_ptr);
	}
}

/**
 * @brief The entry point of the minishell program.
 *
 * This function initializes the shell's state, sets up signal handlers,
 * and starts the main interactive loop.
 * - It ignores the command-line arguments `argc` and `argv`.
 * - `set_interactive_signals()` sets up handlers for signals like
 * SIGINT and SIGQUIT.
 * - `env_init(envp)` creates a linked list to store environment variables.
 * - The `t_shell` struct is initialized with default values.
 * - `input_loop()` is called to start the shell's main interactive loop.
 * - After the loop exits, `reset_signals()` restores default signal handlers,
 *   and allocated memory is freed before the program exits with the last
 *   command's exit status.
 *
 * @param argc The number of command-line arguments (unused).
 * @param argv The array of command-line arguments (unused).
 * @param envp The array of environment variables from the parent process.
 * @return The final exit status of the shell.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	set_interactive_signals();
	shell.env = env_init(envp);
	shell.command = NULL;
	shell.tokens = NULL;
	shell.redir = NULL;
	shell.line = NULL;
	shell.exit_status = 0;
	input_loop(&shell);
	reset_signals();
	rl_clear_history();
	close_heredoc_pipes(&shell);
	free(shell.line);
	free_env(shell.env);
	return (shell.exit_status);
}
