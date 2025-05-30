/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:03:02 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Determines and initiates the execution of a command or pipeline.
 *
 * This function serves as the main entry point for command execution after
 * parsing. It first checks if there is any command to execute
 * (`shell->command`). If not, it returns.
 *
 * If there is a single command (`!shell->command->next`):
 *   - It checks if the command is a built-in using `is_builtin()`.
 *   - If it's a built-in, `exec_builtin()` is called.
 *   - Otherwise (it's an external command), `exec_external()` is called.
 *   The exit status from these functions is stored in `shell->exit_status`.
 *
 * If there are multiple commands
 * (i.e., a pipeline, `shell->command->next` is true):
 *   - `execute_pipe()` is called to handle the pipeline execution.
 *   The exit status from `execute_pipe()` is stored in `shell->exit_status`.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the
 *              parsed command(s) and other shell state information.
 */
void	run_command(t_shell *shell)
{
	if (!shell->command)
		return ;
	if (!shell->command->next)
	{
		if (is_builtin(shell->command->cmd))
			shell->exit_status = exec_builtin(shell);
		else
			shell->exit_status = exec_external(shell);
	}
	else
		shell->exit_status = execute_pipe(shell);
}
