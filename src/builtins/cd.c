/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/11 17:22:53 by bkiskac          ###   ########.fr       */
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
		return (ft_putendl_fd("cd: HOME not set", STDERR_FILENO), NULL);
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

	if (argc < 2)
	{
		raw = get_env_value("HOME", *env);
		if (!raw)
			return (ft_putendl_fd("cd: HOME not set", STDERR_FILENO), NULL);
		return (raw);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		raw = get_env_value("OLDPWD", *env);
		if (!raw)
			return (ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO), NULL);
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
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (ERROR);
	}
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
		return (perror("getcwd error"), ERROR);
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
		return (ft_putendl_fd("cd: too many arguments", 2), ERROR);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd && errno != ENOENT)
		return (perror("getcwd error"), ERROR);
	target = get_target(argc, args, env);
	if (!target)
		return (ERROR);
	if (change_directory(target, &new_pwd) == ERROR)
		return (free(old_pwd), free(target), ERROR);
	if (argc >= 2 && ft_strcmp(args[1], "-") == 0)
		ft_putendl_fd(new_pwd, STDOUT_FILENO);
	if (update_env("OLDPWD", old_pwd, env) == ERROR
		|| update_env("PWD", new_pwd, env) == ERROR)
	{
		ft_putstr_fd("minishell: cd: failed to update environment\n", 2);
		return (free(old_pwd), free(new_pwd), free(target), ERROR);
	}
	return (free(old_pwd), free(new_pwd), free(target), 0);
}
