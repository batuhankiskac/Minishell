/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:04:16 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/08 22:00:39 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * builtin_env - Executes the env builtin command.
 *
 * This function iterates through the environment linked list
 * and prints each variable
 * in the format "key=value" to the standard output.
 *
 * Parameters:
 *   env - A pointer to the head of the environment variables linked list.
 *
 * Returns:
 *   0 - Upon successful execution.
 */
int	builtin_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
