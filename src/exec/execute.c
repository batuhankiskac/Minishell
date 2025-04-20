/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:03:10 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/20 15:03:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* run_command - execute the command in the shell context
*
* Checks if the command is a builtin or an external command and executes it.
* If the command is a pipeline, it calls execute_pipe to handle the pipeline.
*
* Parameters:
*   t_shell *shell    - shell context containing the command to execute
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
