/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:22:51 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 13:36:15 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* env_init - Initializes a linked list of environment variables.
*
* This function processes an array of environment strings (envp) where each
* string is expected
* to be in the "KEY=VALUE" format. It creates a new linked list node for
* each valid entry by:
*   - Locating the '=' character to determine the split
* point between key and value.
*   - Creating substrings for the key (everything before '=')
* and the value (everything after '=').
*   - Adding the new node at the beginning of the list.
*
* Parameters:
*   char *envp[] - An array of environment variable strings.
*
* Returns:
*   A pointer to the head of the constructed linked list
* of t_env nodes, or NULL if memory allocation fails.
*/
t_env	*env_init(char *envp[])
{
	t_env	*env;
	t_env	*new_node;
	char	*equal_sign;
	int		i;

	env = NULL;
	new_node = NULL;
	i = -1;
	while (envp[++i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (!equal_sign)
			continue ;
		new_node = (t_env *)safe_malloc(sizeof(t_env));
		new_node->key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		new_node->value = ft_strdup(equal_sign + 1);
		if (!new_node->key || !new_node->value)
			return (free_env(env), free(new_node), NULL);
		new_node->next = env;
		env = new_node;
	}
	return (env);
}
