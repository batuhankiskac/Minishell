/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 19:04:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		exit(EXIT_FAILURE);
	}
}
