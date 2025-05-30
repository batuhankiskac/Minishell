/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 20:53:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the value of an existing environment variable node.
 *
 * This function is called when an environment variable already exists and its
 * value needs to be updated. It duplicates the new `value` (if not NULL)
 * and assigns it to `node->value`, after freeing the old `node->value`.
 * If `value` is NULL, `node->value` is set to NULL.
 * If `ft_strdup` fails, the function returns without changing the value,
 * potentially leaving the old value intact or causing an issue if the old
 * value was already freed. (Note: Current implementation frees old value first).
 *
 * @param value The new value for the environment variable. Can be NULL.
 * @param node A pointer to the `t_env` node whose value is to be updated.
 */
static void	update_existing_env(char *value, t_env *node)
{
	char	*new_value;

	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return ;
	}
	else
	{
		new_value = NULL;
	}
	free(node->value);
	node->value = new_value;
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
 */
void	update_env(char *key, char *value, t_env **env)
{
	t_env	*node;

	node = find_env(key, *env);
	if (node)
	{
		update_existing_env(value, node);
		return ;
	}
	node = safe_malloc(sizeof(t_env));
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = *env;
	*env = node;
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return ;
	}
}
