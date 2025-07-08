/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 11:12:58 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses key and value from export argument.
 *
 * This function splits an argument into key and value components.
 * Handles both "VAR=value" and "VAR" formats.
 *
 * @param arg The argument string.
 * @param key Pointer to store the extracted key.
 * @param value Pointer to store the extracted value (NULL if no '=').
 * @return 0 on success, ERROR on failure.
 */
static int	parse_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		if (!*key)
			return (ERROR);
		*value = ft_strdup(equal + 1);
		if (!*value)
		{
			free(*key);
			return (ERROR);
		}
	}
	else
	{
		*key = ft_strdup(arg);
		if (!*key)
			return (ERROR);
		*value = NULL;
	}
	return (0);
}

/**
 * @brief Updates environment with validated key-value pair.
 *
 * This function handles the environment update logic based on
 * whether a value is provided or not.
 *
 * @param key The environment variable key.
 * @param value The environment variable value (NULL if no value).
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	update_export_env(char *key, char *value, t_env **env)
{
	if (value == NULL)
	{
		if (!find_env(key, *env))
		{
			if (update_env(key, NULL, env) == ERROR)
				return (1);
			return (0);
		}
	}
	else
	{
		if (update_env(key, value, env) == ERROR)
			return (1);
		return (0);
	}
	return (0);
}

/**
 * @brief Processes a single export argument.
 *
 * This function validates and processes an export argument,
 * updating the environment accordingly.
 *
 * @param arg The argument string.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		ret;

	if (parse_key_value(arg, &key, &value) == ERROR)
		return (print_error("export", arg, "not a valid identifier", 1));
	if (!is_valid_identifier(key))
	{
		free(key);
		free(value);
		return (print_error("export", arg, "not a valid identifier", 1));
	}
	ret = update_export_env(key, value, env);
	free(key);
	free(value);
	return (ret);
}

/**
 * @brief Implements the built-in export command.
 *
 * This function handles the export command. If no arguments are provided,
 * it prints the sorted environment variables. If arguments are provided,
 * it processes each argument to add or update environment variables.
 * It returns a non-zero status code if any argument is invalid.
 *
 * @param argc The number of arguments passed to the export command.
 * @param args The array of arguments passed to the export command.
 * @param env A pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	builtin_export(int argc, char **args, t_env **env)
{
	int		i;
	int		ret;

	if (argc == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 0;
	ret = 0;
	while (++i < argc)
		if (process_export_arg(args[i], env) != 0)
			ret = 1;
	return (ret);
}
