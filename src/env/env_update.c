/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:46:50 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/08 21:56:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Update the value for an existing env node.
* The function duplicates the new value, frees the old one,
* and assigns the new value.
*/
static void	update_existing_env(char *value, t_env *node)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return ;
	free(node->value);
	node->value = new_value;
}

/*
* Searches for the environment variable node matching the provided key.
*
* If a matching node is found, it updates its value using update_existing_env.
* If not, it allocates a new node with the given key and value, and appends it
* to the environment list.
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
	node->value = ft_strdup(value);
	node->next = *env;
	*env = node;
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		exit(EXIT_FAILURE);
	}
}
