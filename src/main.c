/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 19:11:24 by bkiskac          ###   ########.fr       */
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

	// Test 1: Basic redirection test
	redir_node.type = REDIR_OUT;
	redir_node.file = "test_output.txt";
	redir_node.next = NULL;

	cmd.cmd = "echo";  // Dummy komut adı.
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

	printf("Hello, redirection test!\n");

	// Test 2: Valgrind test (to be run externally)
	// Run this program with valgrind to check for memory leaks.

	// Test 3: Child process test
	pid_t pid = fork();
	if (pid == 0) // Child process
	{
		if (setup_redir(&shell) == ERROR)
		{
			fprintf(stderr, "Child: Redirection setup failed\n");
			exit(1);
		}
		printf("Child process redirection test!\n");
		exit(0);
	}
	else if (pid > 0) // Parent process
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("Child exited with status %d\n", WEXITSTATUS(status));
		}
		else
		{
			fprintf(stderr, "Child process did not exit normally\n");
		}
	}
	else
	{
		fprintf(stderr, "Fork failed\n");
		return (1);
	}

	return (0);
}
