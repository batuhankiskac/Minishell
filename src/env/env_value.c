/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/10 09:02:03 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is valid for an environment variable name.
 *
 * This function checks if the given character is alphanumeric or an underscore.
 * It is used to validate characters in environment variable names.
 *
 * @param c The character to check.
 * @return 1 if the character is valid, 0 otherwise.
 */
int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * @brief Extracts the value part of an environment string.
 *
 * This function takes a string in the format "KEY=VALUE" and returns
 * a dynamically allocated copy of the VALUE part. If the string does
 * not contain an '=', NULL is returned.
 *
 * @param env The environment string to parse.
 * @return A dynamically allocated string containing the value, or NULL
 *         if no '=' is found.
 */
char	*env_value(char *env)
{
	int		i;
	char	*value;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (!env[i])
		return (NULL);
	value = ft_strdup(env + i + 1);
	return (value);
}

/**
 * @brief Retrieves the value of an environment variable by its key.
 *
 * This function searches the environment linked list for a variable
 * with the specified key and returns a dynamically allocated copy of
 * its value.
 *
 * @param key The key of the environment variable to retrieve.
 * @param env A pointer to the environment linked list.
 * @return A dynamically allocated string containing the value of the
 *         environment variable, or NULL if the key is not found.
 */
char	*get_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}
