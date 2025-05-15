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

int	builtin_export(int argc, char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*value;
	i = 0;
	if (argc == 1)
		return (print_sorted_env(*env), 0);
	while (++i < argc)
	{
		if (parse_export_arg(args[i], &key, &value) == ERROR)
		{
			ft_putstr_fd("export: not a valid identifier: ", 2);
			ft_putendl_fd(args[i], 2);
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

