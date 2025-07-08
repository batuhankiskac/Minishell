/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 11:10:59 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new environment node from an environment variable string.
 *
 * This function creates a new t_env node by parsing an environment variable
 * string (format: "KEY=VALUE") and extracting the key and value parts.
 *
 * @param env_var The environment variable string to parse.
 * @return A pointer to the newly created environment node,
 * or NULL if an error occurs.
 */
static t_env	*create_env_node(char *env_var)
{
	t_env	*new_node;
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (!equal_sign)
		return (NULL);
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (print_error(NULL, NULL, strerror(errno), -127));
	new_node->key = ft_substr(env_var, 0, equal_sign - env_var);
	new_node->value = ft_strdup(equal_sign + 1);
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

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
	int		i;

	env = NULL;
	i = -1;
	while (envp[++i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			if (ft_strchr(envp[i], '='))
			{
				free_env(env);
				return (NULL);
			}
			continue ;
		}
		new_node->next = env;
		env = new_node;
	}
	return (env);
}
