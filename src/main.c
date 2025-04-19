/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/19 21:57:54 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_command	cmd;
	t_redir	redir_node;
	int		orig_stdout;

	/* ignore unused params */
	(void)argc;
	(void)argv;

	/* STDOUT'un orijinal tanımlayıcısını yedekleyelim */
	orig_stdout = dup(STDOUT_FILENO);
	if (orig_stdout < 0)
	{
		perror("dup");
		return (1);
	}

	/* Test için output redirection ayarları:
	   Bu testte, STDOUT 'test_output.txt' dosyasına yönlendirilecek */
	redir_node.type = REDIR_OUT;
	redir_node.file = "test_output.txt";
	redir_node.next = NULL;

	cmd.cmd = "echo";  /* Dummy komut adı, burada önemli olan redirection */
	cmd.args = NULL;
	cmd.argc = 0;
	cmd.redir = &redir_node;

	/* init environment list */
	shell.env = env_init(envp);
	if (!shell.env)
		fprintf(stderr, "env_init failed\n");

	shell.command = &cmd;
	shell.exit_status = 0;

	if (setup_redir(&shell) == ERROR)
	{
		fprintf(stderr, "Redirection setup failed\n");
		return (1);
	}

	/* Redirection aktifken, yazdırılan çıktı dosyaya gitmeli */
	printf("Hello, redirection test!\n");
	fflush(stdout);  /* Tamponu boşaltır, böylece çıktı hemen dosyaya yazılır */

	/* STDOUT'u yeniden terminale döndürelim */
	if (dup2(orig_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 restore");
		close(orig_stdout);
		return (1);
	}
	close(orig_stdout);

	/* Bu çıktı terminalde görünmeli */
	printf("Hello, terminal output!\n");

	/* save initial cwd for later restoration */
	char *init_cwd = getcwd(NULL, 0);

	/* Automated test suite */
	printf("\n=== Starting automated tests ===\n");

	/* Test echo builtin */
	{
		t_command test_cmd;
		char *args[] = {"echo", "Test echo", NULL};
		test_cmd.cmd = "echo";
		test_cmd.args = args;
		test_cmd.argc = 2;
		test_cmd.redir = NULL;
		printf("\n>>> echo builtin: ");
		shell.command = &test_cmd;
		exec_builtin(&shell);
	}

	/* Test env builtin */
	{
		t_command test_cmd;
		char *args[] = {"env", NULL};
		test_cmd.cmd = "env";
		test_cmd.args = args;
		test_cmd.argc = 1;
		test_cmd.redir = NULL;
		printf("\n>>> env builtin:\n");
		shell.command = &test_cmd;
		exec_builtin(&shell);
	}

	/* Test export and unset */
	{
		t_command test_cmd;
		char *args1[] = {"export", "MYVAR=VALUE", NULL};
		test_cmd.cmd = "export";
		test_cmd.args = args1;
		test_cmd.argc = 2;
		test_cmd.redir = NULL;
		printf("\n>>> export builtin: ");
		shell.command = &test_cmd;
		exec_builtin(&shell);

		char *args2[] = {"env", NULL};
		test_cmd.cmd = "env";
		test_cmd.args = args2;
		test_cmd.argc = 1;
		printf("\n>>> env after export:\n");
		shell.command = &test_cmd;
		exec_builtin(&shell);

		char *args3[] = {"unset", "MYVAR", NULL};
		test_cmd.cmd = "unset";
		test_cmd.args = args3;
		test_cmd.argc = 2;
		printf("\n>>> unset builtin: ");
		shell.command = &test_cmd;
		exec_builtin(&shell);

		char *args4[] = {"env", NULL};
		test_cmd.cmd = "env";
		test_cmd.args = args4;
		test_cmd.argc = 1;
		printf("\n>>> env after unset:\n");
		shell.command = &test_cmd;
		exec_builtin(&shell);
	}

	/* Test cd builtin */
	{
		t_command test_cmd;
		char *args1[] = {"cd", "/", NULL};
		test_cmd.cmd = "cd";
		test_cmd.args = args1;
		test_cmd.argc = 2;
		test_cmd.redir = NULL;
		printf("\n>>> cd to root: ");
		shell.command = &test_cmd;
		exec_builtin(&shell);
		char *cwd = getcwd(NULL, 0);
		printf("PWD: %s\n", cwd);
		free(cwd);

		char *args2[] = {"cd", "/nonexistent", NULL};
		test_cmd.cmd = "cd";
		test_cmd.args = args2;
		test_cmd.argc = 2;
		printf("\n>>> cd to nonexistent: ");
		shell.command = &test_cmd;
		exec_builtin(&shell);
	}

	/* restore original cwd before redirection tests */
	if (init_cwd && chdir(init_cwd) == -1)
		perror("restore cwd");
	free(init_cwd);

	/* Test external command */
	{
		t_command test_cmd;
		char *args[] = {"ls", "-l", NULL};
		test_cmd.cmd = "ls";
		test_cmd.args = args;
		test_cmd.argc = 2;
		test_cmd.redir = NULL;
		printf("\n>>> external ls command:\n");
		shell.command = &test_cmd;
		exec_external(&shell);
	}

	/* Test redirection with echo */
	{
		t_redir red;
		red.type = REDIR_OUT;
		red.file = "redir_echo.txt";
		red.next = NULL;
		t_command test_cmd;
		char *args[] = {"echo", "Redirected", NULL};
		test_cmd.cmd = "echo";
		test_cmd.args = args;
		test_cmd.argc = 2;
		test_cmd.redir = &red;
		printf("\n>>> redirection test for echo (output to redir_echo.txt)\n");
		shell.command = &test_cmd;
		if (setup_redir(&shell) == ERROR)
			fprintf(stderr, "redirection setup failed for test\n");
		exec_builtin(&shell);
		dup2(orig_stdout, STDOUT_FILENO);
	}

	/* Test append redirection */
	{
		FILE *f = fopen("append_input.txt", "w");
		fprintf(f, "Existing line\n");
		fclose(f);
		t_redir red_app;
		red_app.type = REDIR_APPEND;
		red_app.file = "append_echo.txt";
		red_app.next = NULL;
		t_command test_cmd2;
		char *args_app[] = {"echo", "Appended", NULL};
		test_cmd2.cmd = "echo";
		test_cmd2.args = args_app;
		test_cmd2.argc = 2;
		test_cmd2.redir = &red_app;
		printf("\n>>> append redir echo: ");
		shell.command = &test_cmd2;
		if (setup_redir(&shell) == ERROR)
			fprintf(stderr, "append redir setup failed\n");
		exec_builtin(&shell);
		dup2(orig_stdout, STDOUT_FILENO);
		printf("\n>>> content of append_echo.txt:\n");
		system("cat append_echo.txt");
	}

	/* Test input redirection */
	{
		FILE *f = fopen("input_test.txt", "w");
		fprintf(f, "InputTest\n");
		fclose(f);
		t_redir red_in;
		red_in.type = REDIR_IN;
		red_in.file = "input_test.txt";
		red_in.next = NULL;
		t_command test_cmd3;
		char *args_in[] = {"cat", NULL};
		test_cmd3.cmd = "cat";
		test_cmd3.args = args_in;
		test_cmd3.argc = 1;
		test_cmd3.redir = &red_in;
		printf("\n>>> input redir external cat:\n");
		shell.command = &test_cmd3;
		if (setup_redir(&shell) == ERROR)
			fprintf(stderr, "input redir setup failed\n");
		exec_external(&shell);
		dup2(orig_stdout, STDOUT_FILENO);
	}

	printf("\n=== Tests completed ===\n");

	/* Free environment list to avoid memory leaks */
	free_env(shell.env);

	return (0);
}
