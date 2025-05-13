/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:28:16 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/08 21:56:42 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Checks if a character is valid in env keys.
*
* Valid characters include all alphanumerics and the underscore (_).
*/
int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
*  Determines if the env string is in KEY=VALUE format.
*  Returns 1 if valid, otherwise returns 0.
*/
int	is_valid_env(char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!is_env_char(str[i]))
			return (0);
		i++;
	}
	if (str[i] != '=')
		return (0);
	return (1);
}

/*
* Calculates the length of the VALUE part.
* It counts the characters after the '=' sign.
*/
int	env_value_len(char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	while (str[++i])
		len++;
	return (len);
}

/*
* Returns a duplicate of the VALUE part.
*
* If the '=' sign is missing, returns NULL.
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

/*
* Gets the duplicate of the VALUE for a given key.
* Walks the env list; returns NULL if not found.
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
