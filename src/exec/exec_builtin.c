/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:16:06 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 16:31:50 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_builtin(t_shell *shell)
{
	if (!ft_strcmp(shell->command->cmd, "cd"))
		return (builtin_cd(shell->command->argc,
				shell->command->args, &shell->env));
	else if (!ft_strcmp(shell->command->cmd, "echo"))
		return (builtin_echo(shell->command->argc, shell->command->args));
	else if (!ft_strcmp(shell->command->cmd, "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(shell->command->cmd, "export"))
		return (builtin_export(shell->command->argc,
				shell->command->args, &shell->env));
	else if (!ft_strcmp(shell->command->cmd, "unset"))
		return (builtin_unset(shell->command->argc,
				shell->command->args, &shell->env));
	else if (!ft_strcmp(shell->command->cmd, "env"))
		return (builtin_env(&shell->env));
	else if (!ft_strcmp(shell->command->cmd, "exit"))
		return (builtin_exit(shell->command->argc,
				shell->command->args, &shell->env));
	return (0);
}
