/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/11 15:20:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the value of an existing environment variable node.
 *
 * This function safely updates an environment variable's value by first
 * attempting to allocate memory for the new value before freeing the old one.
 * This prevents data loss if memory allocation fails.
 *
 * @param value The new value for the environment variable. Can be NULL.
 * @param node A pointer to the `t_env` node whose value is to be updated.
 * @return 0 on success, ERROR (-1) on memory allocation failure.
 */
static int	update_existing_env(char *value, t_env *node)
{
	char	*new_value;
	char	*old_value;

	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
		{
			perror("minishell: export: memory allocation failed");
			return (ERROR);
		}
	}
	else
	{
		new_value = NULL;
	}
	old_value = node->value;
	node->value = new_value;
	return (free(old_value), 0);
}

/**
 * @brief Updates or adds an environment variable.
 *
 * This function searches for an environment variable by its key. If the
 * variable exists, its value is updated. If it does not exist, a new
 * variable is added to the environment list.
 *
 * @param key The key of the environment variable to update or add.
 * @param value The value to set for the environment variable (can be NULL).
 * @param env A pointer to the environment list.
 * @return 0 on success, ERROR (-1) on failure.
 */
int	update_env(char *key, char *value, t_env **env)
{
	t_env	*node;

	node = find_env(key, *env);
	if (node)
		return (update_existing_env(value, node));
	node = malloc(sizeof(t_env));
	if (!node)
		return (perror("minishell: malloc error"), ERROR);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (perror("minishell: malloc error"), ERROR);
	}
	node->next = *env;
	*env = node;
	return (0);
}
