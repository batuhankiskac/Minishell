/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:22:51 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/04 14:42:02 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env *env_init(char *envp[])
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
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (perror("malloc error"), NULL);
	}
}
