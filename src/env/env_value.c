/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:32:43 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

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

