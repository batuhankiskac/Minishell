/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 20:08:57 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell	shell;
	t_command	cmd;
	int		exit_status;

	/* Environment linked listesini oluştur */
	shell.env = env_init(envp);
	if (!shell.env)
	{
		ft_putendl_fd("env_init failed", 2);
		return (1);
	}

	/* External komut testi: "ls -l" */
	cmd.cmd = ft_strdup("ls");
	cmd.args = (char *[]){"ls", "-l", NULL};
	cmd.argc = 2;
	shell.command = &cmd;
	exit_status = exec_external(&shell);
	ft_putstr_fd("External command exit status: ", 1);
	ft_putnbr_fd(exit_status, 1);
	ft_putchar_fd('\n', 1);
	free(cmd.cmd);

	/* Tüm environment kaynaklarını temizle */
	free_env(shell.env);
	return (0);
}
