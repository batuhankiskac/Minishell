/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:26:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 19:03:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;

	env = env_init(envp);
	if (!env)
	{
		ft_putendl_fd("Env initialization failed", 2);
		return (1);
	}
	builtin_export(2, (char *[]){"export", "TEST=hello"}, &env);
	ft_putendl_fd("Current Environment:", 1);
	builtin_env(env);
	builtin_unset(2, (char *[]){"unset", "TEST"}, &env);
	ft_putendl_fd("After unset:", 1);
	builtin_env(env);
	builtin_cd(2, (char *[]){"cd", ".."}, &env);
	ft_putendl_fd("After cd:", 1);
	builtin_env(env);
	free_env(env);
	return (0);
}
