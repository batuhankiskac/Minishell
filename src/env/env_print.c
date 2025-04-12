/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:52:58 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/12 16:57:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* Computes and returns the number of strings in the given array.
* The function stops counting when a NULL pointer is found.
*/
static int	str_array_len(char **array)
{
	int	len;

	len = 0;
	while (array[len])
		len++;
	return (len);
}

/*
* Sorts the 'env_array' using a simple bubble-sort algorithm.
* The array is reordered in lexicographic ascending order.
*/
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

/*
* Converts the environment linked list 'env' to a string array,
* sorts the array alphabetically, and prints each element with the
* prefix "declare -x" to standard output.
*/
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
		printf("declare -x %s\n", env_array[i]);
	ft_free_all(env_array);
}
