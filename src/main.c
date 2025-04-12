/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 19:27:51 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell	shell;
	t_command	cmd;
	int		ret;

	/* Environment linked listesini oluştur */
	shell.env = env_init(envp);
	if (!shell.env)
	{
		ft_putendl_fd("env_init failed", 2);
		return (1);
	}

	/* Test built-in: echo */
	cmd.cmd = ft_strdup("echo");
	{
		char *echo_args[] = {"echo", "Hello, world!", NULL};
		cmd.args = echo_args;
		cmd.argc = 2;
		shell.command = &cmd;
		ret = exec_builtin(&shell);
		printf("echo returned: %d\n", ret);
		free(cmd.cmd);
	}

	/* Test built-in: pwd */
	cmd.cmd = ft_strdup("pwd");
	{
		char *pwd_args[] = {"pwd", NULL};
		cmd.args = pwd_args;
		cmd.argc = 1;
		shell.command = &cmd;
		ret = exec_builtin(&shell);
		printf("pwd returned: %d\n", ret);
		free(cmd.cmd);
	}

	/* Test built-in: env */
	cmd.cmd = ft_strdup("env");
	{
		char *env_args[] = {"env", NULL};
		cmd.args = env_args;
		cmd.argc = 1;
		shell.command = &cmd;
		ret = exec_builtin(&shell);
		printf("env returned: %d\n", ret);
		free(cmd.cmd);
	}

	/* Test built-in: export */
	cmd.cmd = ft_strdup("export");
	{
		char *export_args[] = {"export", "TEST=hello", NULL};
		cmd.args = export_args;
		cmd.argc = 2;
		shell.command = &cmd;
		ret = exec_builtin(&shell);
		printf("export returned: %d\n", ret);
		free(cmd.cmd);
	}

	/* Test built-in: unset */
	cmd.cmd = ft_strdup("unset");
	{
		char *unset_args[] = {"unset", "TEST", NULL};
		cmd.args = unset_args;
		cmd.argc = 2;
		shell.command = &cmd;
		ret = exec_builtin(&shell);
		printf("unset returned: %d\n", ret);
		free(cmd.cmd);
	}

	free_env(shell.env);
	return (0);
}
