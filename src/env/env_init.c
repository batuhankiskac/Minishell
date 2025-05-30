/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:57:57 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the environment linked list from envp.
 *
 * This function creates a linked list of environment variables from
 * the provided envp array. Each variable is split into a key and value
 * pair, and a new node is added to the list for each valid entry.
 *
 * @param envp The array of environment variables (e.g., from main).
 * @return A pointer to the head of the environment linked list, or NULL
 *         if an error occurs during initialization.
 */
t_env	*env_init(char *envp[])
{
	t_env	*env;
	t_env	*new_node;
	char	*equal_sign;
	int		i;

	env = NULL;
	new_node = NULL;
	i = -1;
	while (envp[++i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (!equal_sign)
			continue ;
		new_node = (t_env *)safe_malloc(sizeof(t_env));
		new_node->key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		new_node->value = ft_strdup(equal_sign + 1);
		if (!new_node->key || !new_node->value)
			return (free_env(env), free(new_node), NULL);
		new_node->next = env;
		env = new_node;
	}
	return (env);
}
