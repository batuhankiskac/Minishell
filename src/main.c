/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 19:30:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	t_shell	shell;
	t_command	cmd;
	t_redir	redir_node;
	int		orig_stdout;

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

	shell.env = NULL;
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

	return (0);
}
