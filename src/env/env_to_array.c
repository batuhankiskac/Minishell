/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 23:55:56 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of nodes in an environment linked list.
 *
 * This function iterates through the `t_env` linked list pointed to by `env`
 * and counts the number of nodes until it reaches the end (NULL).
 *
 * @param env A pointer to the head of the `t_env` linked list.
 * @return The total number of environment variables (nodes) in the list.
 */
static int	env_count(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Converts a single environment node (t_env) to a "KEY=VALUE" string.
 *
 * This function takes a pointer to a `t_env` node.
 * If `env_node->value` is not NULL, it allocates memory and constructs a string
 * in the format "KEY=VALUE". The length is calculated to include the key,
 * the '=' sign, the value, and a null terminator.
 * If `env_node->value` is NULL, it allocates memory and constructs a string
 * containing only the KEY.
 * It uses `ft_strlcpy` and `ft_strlcat` for safe string construction or
 * `ft_strdup` if only the key is present.
 *
 * @param env_node A pointer to the `t_env` node to be converted.
 * @return A dynamically allocated string representing the environment variable.
 *         Returns NULL if memory allocation fails.
 */
static char	*env_node_to_string(t_env *env_node)
{
	char	*str;
	int		len;

	if (env_node->value)
	{
		len = ft_strlen(env_node->key) + ft_strlen(env_node->value) + 2;
		str = (char *)malloc(sizeof(char) * len);
		if (!str)
			return (NULL);
		ft_strlcpy(str, env_node->key, len);
		ft_strlcat(str, "=", len);
		ft_strlcat(str, env_node->value, len);
	}
	else
	{
		str = ft_strdup(env_node->key);
		if (!str)
			return (NULL);
	}
	return (str);
}

/**
 * @brief Converts the environment linked list to an array.
 *
 * This function iterates through the environment linked list and creates
 * an array of strings, where each string is in the format "KEY=VALUE".
 * Variables without a value are represented as "KEY".
 *
 * @param env A pointer to the environment linked list.
 * @return A dynamically allocated array of strings representing the
 *         environment variables, or NULL if an error occurs.
 */
char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	char	*str;
	int		i;

	i = 0;
	env_array = (char **)malloc(sizeof(char *) * (env_count(env) + 1));
	if (!env_array)
	{
		print_error(NULL, NULL, strerror(errno), 0);
		return (NULL);
	}
	while (env)
	{
		str = env_node_to_string(env);
		if (!str)
		{
			ft_free_all(env_array);
			return (NULL);
		}
		env_array[i++] = str;
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
