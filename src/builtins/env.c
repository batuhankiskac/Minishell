/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/31 14:54:45 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the built-in env command.
 *
 * This function prints the current environment variables.
 * It iterates through the environment list and prints each variable
 * in the format KEY=VALUE. Variables without a value are printed as KEY=.
 *
 * @param env A pointer to the environment list.
 * @return Always returns 0.
 */
int	builtin_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->key && current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(current->value, STDOUT_FILENO);
		}
		else if (current->key && !current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}
