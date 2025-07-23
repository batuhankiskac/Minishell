/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:58:05 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/23 17:57:01 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the execution of a command that only contains redirections.
 *
 * This function is a specialist for cases like "> out" or "<< EOF". It
 * safely backs up and restores STDIN/STDOUT while applying the redirections.
 * @param shell A pointer to the t_shell structure.
 * @return Returns 1 on redirection error, 0 on success.
 */
static int	execute_redir_only(t_shell *shell)
{
	int	status;
	int	original_stdin;
	int	original_stdout;

	status = 0;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (setup_redir(shell) == ERROR)
		status = 1;
	if (original_stdin != -1)
		dup_fd(original_stdin, STDIN_FILENO, "stdin");
	if (original_stdout != -1)
		dup_fd(original_stdout, STDOUT_FILENO, "stdout");
	return (status);
}

/**
 * @brief Dispatches a single (non-piped) command to the correct executor.
 *
 * This function determines if the command is a redirection-only command,
 * a built-in, or an external command, and calls the appropriate function.
 * An empty string command ("") is correctly passed to the external command
 * handler to produce a "command not found" error.
 *
 * @param shell A pointer to the t_shell structure.
 * @return The exit status of the executed command.
 */
static int	execute_single_command(t_shell *shell)
{
	if (!shell->command->cmd)
		return (execute_redir_only(shell));
	else if (is_builtin(shell->command->cmd))
		return (exec_builtin(shell));
	else
		return (exec_external(shell));
}

/**
 * @brief The main entry point for command execution.
 *
 * This function acts as a high-level dispatcher. It sets up signals for
 * execution and decides whether to run a pipeline or a single command.
 *
 * @param shell A pointer to the `t_shell` structure.
 */
void	run_command(t_shell *shell)
{
	if (!shell->command)
		return ;
	set_exec_signals();
	if (!shell->command->next)
		shell->exit_status = execute_single_command(shell);
	else
		shell->exit_status = execute_pipe(shell);
	set_interactive_signals();
}
