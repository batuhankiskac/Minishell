/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 23:47:23 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses a single argument for the export command.
 *
 * This function splits an argument string into a key and an optional
 * value based on the first '=' character. It validates the key as a
 * valid identifier.
 *
 * @param arg The argument string (e.g., "VAR=value" or "VAR").
 * @param key A pointer to a char pointer to store the extracted key.
 * @param value A pointer to a char pointer to store the extracted value
 *              (NULL if no '=' is present).
 * @return 0 on success, ERROR on failure (e.g., invalid identifier).
 */
static int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal;
	char	*keystr;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		keystr = ft_substr(arg, 0, equal - arg);
		if (!keystr || !is_valid_identifier(keystr))
		{
			free(keystr);
			return (ERROR);
		}
		*value = ft_strdup(equal + 1);
		if (!*value)
		{
			free(keystr);
			return (ERROR);
		}
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

/**
 * @brief Processes a single export argument.
 *
 * This function parses an export argument, validates it, and updates
 * the environment list accordingly. If the argument has a value, it
 * updates or adds the environment variable. If it has no value, it
 * adds the variable to the environment with a NULL value if it doesn't
 * already exist.
 *
 * @param arg The argument string (e.g., "VAR=value" or "VAR").
 * @param env A pointer to the environment list.
 * @return 0 on success, 1 on failure (e.g., invalid identifier).
 */
static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;

	if (parse_export_arg(arg, &key, &value) == ERROR)
	{
		return (print_error("export", arg, "not a valid identifier", 1));
	}
	if (value == NULL)
	{
		if (!find_env(key, *env))
		{
			if (update_env(key, NULL, env) == ERROR)
			{
				free(key);
				return (1);
			}
		}
	}
	else
	{
		if (update_env(key, value, env) == ERROR)
		{
			free(key);
			free(value);
			return (1);
		}
		free(value);
	}
	free(key);
	return (0);
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
