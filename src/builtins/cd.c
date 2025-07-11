/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/11 11:28:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands tilde (~) in path to HOME directory.
 *
 * @param path The path that may start with ~
 * @param env Environment variables
 * @return Expanded path or NULL on error
 */
static char	*tilde_expansion(char *path, t_env **env)
{
	char	*home;

	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value("HOME", *env);
	if (!home)
		return (print_error_null("cd", NULL, "HOME not set"));
	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
		return (ft_strjoin(home, path + 1));
	return (ft_strdup(path));
}

/**
 * @brief Determines the target directory for the cd command.
 *
 * Parses the arguments provided to the cd command to determine the target
 * directory. If no arguments are given, it returns the value of the HOME
 * environment variable. If the argument is "-", it returns the value of
 * the OLDPWD environment variable. Otherwise, it returns a duplicate of
 * the provided argument. Handles cases where HOME or OLDPWD are not set.
 *
 * @param argc The number of arguments passed to the cd command.
 * @param args The array of arguments passed to the cd command.
 * @param env A pointer to the environment list.
 * @return A dynamically allocated string containing the target path, or
 *         NULL if an error occurs or HOME/OLDPWD is not set.
 */
static char	*get_target(int argc, char **args, t_env **env)
{
	char	*raw;

	if (argc < 2 || (args[1] && args[1][0] == '\0'))
	{
		raw = get_env_value("HOME", *env);
		if (!raw)
			return (print_error_null("cd", NULL, "HOME not set"));
		return (ft_strdup(raw));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		raw = get_env_value("OLDPWD", *env);
		if (!raw)
			return (print_error_null("cd", NULL, "OLDPWD not set"));
		return (ft_strdup(raw));
	}
	return (tilde_expansion(args[1], env));
}

/**
 * @brief Changes the current working directory.
 *
 * Uses the chdir system call to change the directory. Updates the new_pwd
 * pointer with the absolute path of the new current working directory
 * using getcwd. Handles errors during chdir and getcwd.
 *
 * @param target The path to the target directory.
 * @param new_pwd A pointer to a char pointer that will store the new
 *                current working directory path.
 * @return 0 on success, ERROR on failure.
 */
static int	change_directory(char *target, char **new_pwd)
{
	if (chdir(target) == -1)
		return (print_error("cd", target, strerror(errno), 1));
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
		return (print_error("cd", "error retrieving current directory",
				"getcwd: cannot access parent directories", 1));
	return (0);
}

/**
 * @brief Updates environment variables after successful directory change.
 *
 * Updates OLDPWD and PWD environment variables after a successful cd operation.
 *
 * @param old_pwd The previous working directory path.
 * @param new_pwd The new working directory path.
 * @param env A pointer to the environment list.
 * @return 0 on success, ERROR on failure.
 */
static int	update_pwd_env(char *old_pwd, char *new_pwd, t_env **env)
{
	if (update_env("OLDPWD", old_pwd, env) == ERROR)
		return (1);
	if (update_env("PWD", new_pwd, env) == ERROR)
		return (1);
	return (0);
}

/**
 * @brief Implements the built-in cd command.
 *
 * Handles changing the current directory based on the provided arguments.
 * Supports changing to the HOME directory (no arguments), the previous
 * directory ('-'), or a specified path. Updates the PWD and OLDPWD
 * environment variables accordingly. Handles error cases such as too many
 * arguments, directory not found, or HOME/OLDPWD not set.
 *
 * @param argc The number of arguments passed to the cd command.
 * @param args The array of arguments passed to the cd command.
 * @param env A pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	builtin_cd(int argc, char **args, t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*target;

	if (argc > 2)
	{
		ft_printf(2, "cd: too many arguments\n");
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd && errno != ENOENT)
		return (print_error("cd", NULL, strerror(errno), 1));
	target = get_target(argc, args, env);
	if (!target)
		return (cleanup_cd_memory(old_pwd, NULL, NULL, 1));
	if (change_directory(target, &new_pwd) != 0)
		return (cleanup_cd_memory(old_pwd, NULL, target, 1));
	if (argc >= 2 && ft_strcmp(args[1], "-") == 0)
		ft_printf(1, "%s\n", new_pwd);
	if (update_pwd_env(old_pwd, new_pwd, env) == 1)
		return (cleanup_cd_memory(old_pwd, new_pwd, target,
				print_error("cd", NULL, strerror(errno), 1)));
	return (cleanup_cd_memory(old_pwd, new_pwd, target, 0));
}
