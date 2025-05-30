/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:21:31 by bkiskac          ###   ########.fr       */
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
 * @brief Executes the commands in the shell.
 *
 * This function checks if the command is a built-in command or a pipeline.
 * If it's a built-in command, it executes it directly. If it's a pipeline,
 * it calls the `execute_pipe` function to handle the execution of multiple
 * commands connected by pipes.
 *
 * @param shell A pointer to the `t_shell` structure containing the command.
 */
static void	execute_commands(t_shell *shell)
{
	if (shell->command->next == NULL
		&& is_builtin(shell->command->cmd))
		shell->exit_status = exec_builtin(shell);
	else
		shell->exit_status = execute_pipe(shell);
}

/**
 * @brief Processes a single line of input from the user.
 *
 * This function takes a raw line of input, tokenizes it, builds a command list,
 * parses commands and redirections, and executes the commands. It handles errors
 * during parsing and execution, cleaning up resources as necessary.
 *
 * @param raw_line_ptr A pointer to the raw line string entered by the user.
 * @param shell A pointer to the `t_shell` structure containing the shell state.
 * @return Returns 0 on success, or 1 if an error occurs during processing.
 */
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

/**
 * @brief Main loop for processing user input in the shell.
 *
 * This function continuously reads input from the user, processes each line,
 * and executes commands until an EOF signal is received. It resets the signal
 * flag before reading each line to ensure proper signal handling.
 *
 * @param shell A pointer to the `t_shell` structure containing the shell state.
 */
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
	process_input_loop(&shell);
	reset_signals();
	free(shell.line);
	free_env(shell.env);
	return (shell.exit_status);
}
