/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

int	is_valid_identifier(char *str)
{
	int	i;
	if (!str || !*str)
		return (0);
	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!is_env_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}

