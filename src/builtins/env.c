/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:44:13 by bkiskac          ###   ########.fr       */
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
			printf("%s=%s\n", current->key, current->value);
		else if (current->key && !current->value)
			printf("%s=\n", current->key);
		current = current->next;
	}
	return (0);
}
