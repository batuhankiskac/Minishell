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
static void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

static void	delete_env(char *key, t_env **env)
{
	t_env	*current;
	t_env	*prev;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			if (!prev)
				*env = current->next;
			else
				prev->next = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(int argc, char **args, t_env **env)
{
	int	i;
	i = 0;
	if (argc < 2)
		return (0);
	while (++i < argc)
	{
		if (is_valid_identifier(args[i]))
			delete_env(args[i], env);
		else
		{
			ft_putstr_fd("unset: not a valid identifier: ", 2);
			ft_putendl_fd(args[i], 2);
			return (ERROR);
		}
	}
	return (0);
}

