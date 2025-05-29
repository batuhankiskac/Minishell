/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 19:04:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	str_array_len(char **array)
{
	int	len;

	len = 0;
	while (array[len])
		len++;
	return (len);
}

void	sort_env(char **env_array)
{
	int		i;
	int		j;
	int		len;
	char	*temp;

	i = -1;
	len = str_array_len(env_array);
	while (++i < len)
	{
		j = i;
		while (++j < len)
		{
			if (ft_strcmp(env_array[i], env_array[j]) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
		}
	}
}

static void	print_env_var(char *env_str)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
	{
		*equal_pos = '\0';
		printf("declare -x %s=\"%s\"\n", env_str, equal_pos + 1);
		*equal_pos = '=';
	}
	else
	{
		printf("declare -x %s\n", env_str);
	}
}

void	print_sorted_env(t_env *env)
{
	char	**env_array;
	int		i;

	env_array = env_list_to_array(env);
	if (!env_array)
		return ;
	sort_env(env_array);
	i = -1;
	while (env_array[++i])
		print_env_var(env_array[i]);
	ft_free_all(env_array);
}
