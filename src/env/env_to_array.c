/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:32:34 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	char	*str;
	int		i;
	int		len;
	i = 0;
	env_array = (char **)safe_malloc(sizeof(char *) * (env_count(env) + 1));
	while (env)
	{
		len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
		str = (char *)malloc(sizeof(char) * len);
		if (!str)
			return (ft_free_all(env_array), NULL);
		ft_strlcpy(str, env->key, len);
		ft_strlcat(str, "=", len);
		ft_strlcat(str, env->value, len);
		env_array[i++] = str;
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

