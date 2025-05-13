/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_simple_cmds_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:30:21 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/13 21:31:38 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"

int	dummy_builtin(t_tools *tools, t_simple_cmds *simple_cmd)
{
	(void)tools;
	(void)simple_cmd;
	return (0);
}

int	(*builtin_arr(char *str))(t_tools *tools, t_simple_cmds *simple_cmd)
{
	(void)str;
	return (&dummy_builtin);
}

void	single_cmd(t_simple_cmds *cmds, t_tools *tools)
{
	pid_t	pid;

	if (!cmds || !cmds->str || !cmds->str[0])
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(cmds->str[0], cmds->str, tools->envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
	else
		perror("fork");
}
