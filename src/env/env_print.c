/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:58:15 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calculates the length of a null-terminated string array.
 *
 * This function iterates through the `array` until it encounters a NULL
 * pointer, counting the number of non-NULL strings.
 *
 * @param array A null-terminated array of strings.
 * @return The number of strings in the array (excluding the NULL terminator).
 */
static int	str_array_len(char **array)
{
	int	len;

	len = 0;
	while (array[len])
		len++;
	return (len);
}

/**
 * @brief Sorts an array of environment strings lexicographically.
 *
 * This function performs an in-place bubble sort on the `env_array`.
 * Each string in the array is expected to be in the format "KEY=VALUE"
 * or "KEY". The comparison is done using `ft_strcmp`.
 *
 * @param env_array A null-terminated array of strings, where each string
 *                  represents an environment variable.
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

/**
 * @brief Prints a single environment variable in `declare -x` format.
 *
 * This function takes an environment string (`env_str`).
 * If `env_str` contains an '=' (e.g., "KEY=VALUE"), it temporarily splits
 * the string at '=', prints it as `declare -x KEY="VALUE"`, and then
 * restores the '='.
 * If `env_str` does not contain an '=' (e.g., "KEY"), it prints it as
 * `declare -x KEY`.
 *
 * @param env_str A string representing a single environment variable.
 */
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

/**
 * @brief Prints the environment variables in sorted order.
 *
 * This function converts the environment linked list into an array,
 * sorts the array lexicographically, and prints each variable in the
 * format used by the export command. Variables without a value are
 * printed without an equals sign.
 *
 * @param env A pointer to the environment linked list.
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
		print_env_var(env_array[i]);
	ft_free_all(env_array);
}
