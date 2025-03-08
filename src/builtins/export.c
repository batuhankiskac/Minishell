/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:07:50 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/08 18:25:26 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;

	if (!is_valid_env(arg))
		return (ERROR);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = ft_strdup("");
	}
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (0);
}

int	builtin_export(int argc, char *argv[], t_env **env)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	if (argc == 1)
		return (print_sorted_env(*env), 0);
	while (++i < argc)
	{
		if (parse_export_arg(argv[i], &key, &value) == ERROR)
		{
			ft_putstr("export: not a valid identifier: ", 2);
			ft_putendl_fd(argv[i], 2);
			return (ERROR);
		}
		else
		{
			update_env(key, value, env);
			free(key);
			free(value);
		}
	}
	return (0);
}
