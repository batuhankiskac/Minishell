/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:06:25 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:15:09 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	(void)argc;
	(void)argv;

	shell.env = env_init(envp);
	if (!shell.env)
	{
		ft_putstr_fd("Error: Environment initialization failed.\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd("Environment initialized successfully.\n", STDOUT_FILENO);
	ft_putstr_fd("Printing environment variables:\n", STDOUT_FILENO);
	builtin_env(shell.env);
	// Clean up environment
	free_env(shell.env);
	ft_putstr_fd("Environment freed successfully.\n", STDOUT_FILENO);
	return (0);
}
