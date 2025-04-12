/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:24:53 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 16:57:44 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* Traverses the environment variables linked list
* to count the total number of nodes.
*
* This function iterates through the linked list of environment
* variables and counts each node to
* determine the total number of environment variables.
*
* Return int The total count of environment variable nodes.
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

/*
* Convert the env linked list into a NULL-terminated array.
*
* For each node, create a string in the format "key=value"
* and store it in the array.
*
* Return a pointer to the allocated array, or NULL
* if an allocation error occurred.
*/
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
