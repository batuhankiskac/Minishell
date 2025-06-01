/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:59:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the entire environment linked list.
 *
 * This function iterates through the environment linked list and frees
 * the memory allocated for each node, including the key and value strings.
 *
 * @param env A pointer to the head of the environment linked list.
 */
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

/**
 * @brief Finds an environment variable by its key.
 *
 * This function searches the environment linked list for a variable
 * with the specified key and returns a pointer to the corresponding
 * node.
 *
 * @param key The key of the environment variable to find.
 * @param env A pointer to the environment linked list.
 * @return A pointer to the environment node if found, or NULL if not found.
 */
t_env	*find_env(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			return (env);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Checks if a string is a valid environment variable identifier.
 *
 * This function verifies that the given string starts with a non-digit
 * character and contains only valid environment variable characters.
 *
 * @param str The string to validate.
 * @return 1 if the string is a valid identifier, 0 otherwise.
 */
int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
        return (0);
	while (str[i])
	{
		if (!is_env_char(str[i]))
			return (0);
			i++;
	}
	return (1);
}
