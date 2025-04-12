/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 19:03:54 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_command	cmd;
	int		status;
	char	*external_args[] = {"ls", "-l", NULL};
	char	*builtin_args[] = {"env", NULL};

	shell.env = env_init(envp);
	if (!shell.env)
	{
		ft_putendl_fd("Env initialization failed", 2);
		return (1);
	}

	/* External command test: ls -l */
	cmd.cmd = ft_strdup("ls");
	cmd.args = external_args;
	cmd.argc = 2;
	shell.command = &cmd;
	status = exec_external(&shell);
	ft_putstr_fd("External command exit status: ", 1);
	ft_putnbr_fd(status, 1);
	ft_putchar_fd('\n', 1);
	free(cmd.cmd);

	/* Built-in command test: env */
	cmd.cmd = ft_strdup("env");
	cmd.args = builtin_args;
	cmd.argc = 1;
	shell.command = &cmd;
	status = exec_builtin(&shell);
	ft_putstr_fd("Built-in command executed\n", 1);
	free(cmd.cmd);

	free_env(shell.env);
	return (0);
}
