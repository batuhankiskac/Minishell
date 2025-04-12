/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:16:06 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 19:07:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_builtin(t_shell *shell)
{
	if (ft_strcmp(shell->command->cmd, "cd") == 0)
		return (builtin_cd(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "echo") == 0)
		return (builtin_echo(shell->command->argc, shell->command->args));
	else if (ft_strcmp(shell->command->cmd, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(shell->command->cmd, "export") == 0)
		return (builtin_export(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "unset") == 0)
		return (builtin_unset(shell->command->argc,
				shell->command->args, &shell->env));
	else if (ft_strcmp(shell->command->cmd, "env") == 0)
		return (builtin_env(shell->env));
	else if (ft_strcmp(shell->command->cmd, "exit") == 0)
		return (builtin_exit(shell->command->argc,
				shell->command->args, &shell->env));
	return (0);
}
