/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:44:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the memory allocated for a single environment node.
 *
 * This function releases the memory for the key, value, and the node
 * itself.
 *
 * @param node The environment node to free.
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

/**
 * @brief Deletes an environment variable from the list.
 *
 * This function searches for an environment variable by its key and removes
 * the corresponding node from the linked list. It handles freeing the
 * memory of the deleted node.
 *
 * @param key The key of the environment variable to delete.
 * @param env A pointer to the environment list.
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

/**
 * @brief Implements the built-in unset command.
 *
 * This function removes environment variables specified by the arguments
 * from the environment list. It validates each argument as a valid
 * identifier before attempting to delete the corresponding variable.
 *
 * @param argc The number of arguments passed to the unset command.
 * @param args The array of arguments passed to the unset command.
 * @param env A pointer to the environment list.
 * @return 0 on success, ERROR on failure (e.g., invalid identifier).
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
