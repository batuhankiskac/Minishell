/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 20:36:47 by bkiskac          ###   ########.fr       */
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

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	char	*str;
	int		i;

	i = 0;
	env_array = (char **)safe_malloc(sizeof(char *) * (env_count(env) + 1));
	while (env)
	{
		str = env_node_to_string(env);
		if (!str)
			return (ft_free_all(env_array), NULL);
		env_array[i++] = str;
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
