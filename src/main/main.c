/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 22:02:56 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the end-of-file (EOF) signal by printing "exit" and
 *        terminating the shell.
 *
 * This function is called when the user sends an EOF signal (e.g., Ctrl+D).
 * It prints "exit" to indicate that the shell is exiting.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              current shell state.
 */
static void	handle_eof(t_shell *shell)
{
	(void)shell;
	write(STDOUT_FILENO, "exit\n", 5);
}

/**
 * @brief Main loop for processing user input in the shell.
 *
 * This function continuously reads input from the user, processes each line,
 * and executes commands until an EOF signal is received. It resets the signal
 * flag before reading each line to ensure proper signal handling.
 *
 * @param shell A pointer to the `t_shell` structure containing the shell state.
 */
static void	input_loop(t_shell *shell)
{
	char	*raw_line_ptr;

	while (1)
	{
		reset_signal_flag();
		raw_line_ptr = readline("minishell> ");
		if (!raw_line_ptr)
		{
			if (get_signal_flag() == SIGINT)
				continue ;
			handle_eof(shell);
			break ;
		}
		if (process_line(raw_line_ptr, shell) == 1)
			continue ;
	}
}

/**
 * @brief The main function of the minishell program.
 *
 * This function initializes the shell environment, sets up signal handling,
 * and enters the main input processing loop. It cleans up resources before
 * exiting and returns the exit status of the shell.
 *
 * @param argc The number of command-line arguments (not used).
 * @param argv The command-line arguments (not used).
 * @param envp The environment variables passed to the shell.
 * @return The exit status of the shell.
 */
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
	shell.heredoc_eof = 0;
	input_loop(&shell);
	reset_signals();
	free(shell.line);
	free_env(shell.env);
	return (shell.exit_status);
}
