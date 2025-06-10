/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:16:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/10 15:16:53 by bkiskac          ###   ########.fr       */
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
 * This function checks if the command is a single command or a pipeline.
 * For single commands, it determines if it's a built-in or external command
 * and calls the appropriate execution function. For pipelines, it calls
 * the execute_pipe function to handle multiple commands connected by pipes.
 *
 * @param shell A pointer to the `t_shell` structure containing the command.
 */
static void	execute_commands(t_shell *shell)
{
	if (shell->command->next == NULL)
	{
		if (is_builtin(shell->command->cmd))
			shell->exit_status = exec_builtin(shell);
		else
			shell->exit_status = exec_external(shell);
	}
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
int	process_line(char *raw_line_ptr, t_shell *shell)
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
		if (handle_parse_error(raw_line_ptr, shell))
			return (1);
	}
	if (shell->command)
		execute_commands(shell);
	cleanup_loop(raw_line_ptr, shell);
	return (0);
}
