/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 19:04:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal;
	char	*keystr;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		keystr = ft_substr(arg, 0, equal - arg);
		if (!keystr || !is_valid_identifier(keystr))
			return (free(keystr), ERROR);
		*value = ft_strdup(equal + 1);
		if (!*value)
			return (free(keystr), ERROR);
	}
	else
	{
		if (!is_valid_identifier(arg))
			return (ERROR);
		keystr = ft_strdup(arg);
		if (!keystr)
			return (ERROR);
		*value = NULL;
	}
	*key = keystr;
	return (0);
}

static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;

	if (parse_export_arg(arg, &key, &value) == ERROR)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	if (value == NULL)
	{
		if (!find_env(key, *env))
			update_env(key, NULL, env);
	}
	else
	{
		update_env(key, value, env);
		free(value);
	}
	free(key);
	return (0);
}

int	builtin_export(int argc, char **args, t_env **env)
{
	int		i;
	int		ret;

	if (argc == 1)
		return (print_sorted_env(*env), 0);
	i = 0;
	ret = 0;
	while (++i < argc)
	{
		if (process_export_arg(args[i], env) != 0)
			ret = 1;
	}
	return (ret);
}
