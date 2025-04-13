/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 19:04:45 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_shell	shell;
	t_command cmd;
	t_redir	redir_node;

	/* Test için output yönlendirmesi ayarları:
	   Bu testte, komutun STDOUT'u "test_output.txt" dosyasına yönlendirilecek. */
	redir_node.type = REDIR_OUT;
	redir_node.file = "test_output.txt";
	redir_node.next = NULL;

	/* t_command yapısını dolduruyoruz.
	   Burada cmd alanı ve args kullanmayacağız, redirection testinde önemli olan
	   tek şey redir linked list'idir. */
	cmd.cmd = "echo";  // Dummy komut adı.
	cmd.args = NULL;
	cmd.argc = 0;
	cmd.redir = &redir_node;

	/* t_shell yapısını oluşturuyoruz.
	   Environment testimizde kullanılmayacak, bu yüzden env NULL.
	   Komut kısmına t_command'ı atıyoruz. */
	shell.env = NULL;
	shell.command = &cmd;
	shell.exit_status = 0;

	/* Redirection ayarlarını yapıyoruz.
	   Bu fonksiyon open_file() ve dup_fd() gibi yardımcı fonksiyonları çağırarak,
	   REDIR_OUT durumunda STDOUT_FILENO'yu "test_output.txt" dosyasına yönlendirecek. */
	if (setup_redir(&shell) == ERROR)
	{
		fprintf(stderr, "Redirection setup failed\n");
		return (1);
	}

	/* Artık STDOUT, "test_output.txt" dosyasına yönlendirilmiş durumda.
	   Bu printf çıktısı dosyaya yazılmalı. */
	printf("Hello, redirection test!\n");

	/* Programdan çıkmadan önce normalde redirection ayarlandıysa,
	   ilgili dosya tanımlayıcıları zaten dup2 ve close ile yönetildiği için burada ekstra cleanup gerekmez.
	   (Tabii cleanup işlemleri, projenizin diğer bölümlerinde de varsa ek olarak çağrılabilir.) */

	return (0);
}
