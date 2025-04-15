/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:25:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/15 16:13:07 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* free_env_node - Frees a single environment variable node.
*/
static void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

/*
* delete_env - Deletes an environment variable from the linked list.
*
* This helper function removes the specified environment variable from the
* linked list, freeing the memory allocated for the node.
*
* Parameters:
*   key - The key of the environment variable to delete.
*   env - A pointer to the head of the environment variables linked list.
*/
static void	delete_env(char *key, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			if (!prev)
				*env = current->next;
			else
				prev->next = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
 * builtin_unset - Executes the unset builtin command.
 *
 * This function removes the specified environment variables from the
 * environment linked list.
 *
 * Parameters:
 *   env - A pointer to the head of the environment variables linked list.
 *   args - An array of strings containing the names of the variables to unset.
 *
 * Returns:
 *   0 - Upon successful execution.
 */
int	builtin_unset(int argc, char **args, t_env **env)
{
	int	i;

	i = 0;
	if (argc < 2)
		return (0);
	while (++i < argc)
	{
		if (is_valid_identifier(args[i]))
			delete_env(args[i], env);
		else
		{
			ft_putstr_fd("unset: not a valid identifier: ", 2);
			ft_putendl_fd(args[i], 2);
			return (ERROR);
		}
	}
	return (0);
}
