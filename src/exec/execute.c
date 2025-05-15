/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

