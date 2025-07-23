/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/23 21:11:41 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands tilde (~) in path to HOME directory, managing memory safely.
 *
 * @param path The path that may start with ~
 * @param env Environment variables
 * @return Expanded path or NULL on error. The returned string is newly
 * allocated and must be freed by the caller.
 */
static char	*tilde_expansion(char *path, t_env **env)
{
	char	*home;
	char	*expanded_path;

	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value("HOME", *env);
	if (!home)
		return (print_error_null("cd", NULL, "HOME not set"));
	if (path[1] == '\0')
		return (home);
	if (path[1] == '/')
	{
		expanded_path = ft_strjoin(home, path + 1);
		free(home);
		return (expanded_path);
	}
	free(home);
	return (ft_strdup(path));
}

/**
 * @brief Determines the target directory for the cd command.
 *
 * This function consistently returns a newly allocated string which the caller
 * is responsible for freeing. This creates a clear memory ownership contract.
 *
 * @param argc The number of arguments.
 * @param args The array of arguments.
 * @param env A pointer to the environment list.
 * @return A dynamically allocated string containing the target path, or NULL.
 */
static char	*get_target(int argc, char **args, t_env **env)
{
	char	*target_path;

	if (argc < 2 || (args[1] && args[1][0] == '\0'))
	{
		target_path = get_env_value("HOME", *env);
		if (!target_path)
			return (print_error_null("cd", NULL, "HOME not set"));
		return (target_path);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		target_path = get_env_value("OLDPWD", *env);
		if (!target_path)
			return (print_error_null("cd", NULL, "OLDPWD not set"));
		ft_printf(1, "%s\n", target_path);
		return (target_path);
	}
	return (tilde_expansion(args[1], env));
}

/**
 * @brief Changes the current working directory.
 *
 * @param target The path to the target directory.
 * @param new_pwd A pointer to a char pointer that will store the new path.
 * @return 0 on success, 1 on failure.
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
 * @brief Updates OLDPWD and PWD environment variables.
 *
 * @param old_pwd The previous working directory path.
 * @param new_pwd The new working directory path.
 * @param env A pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	update_pwd_env(char *old_pwd, char *new_pwd, t_env **env)
{
	if (old_pwd)
	{
		if (update_env("OLDPWD", old_pwd, env) == ERROR)
			return (1);
	}
	if (update_env("PWD", new_pwd, env) == ERROR)
		return (1);
	return (0);
}

/**
 * @brief Implements the built-in cd command with corrected memory management.
 *
 * This function handles changing the current directory. It ensures that the
 * 'target' path obtained from get_target is always freed via
 * cleanup_cd_memory, closing the identified memory leak.
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
		return (print_error("cd", NULL, "too many arguments", 1));
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd && errno != ENOENT)
		return (print_error("cd", NULL, strerror(errno), 1));
	target = get_target(argc, args, env);
	if (!target)
		return (cleanup_cd_memory(old_pwd, NULL, NULL, 1));
	new_pwd = NULL;
	if (change_directory(target, &new_pwd) != 0)
		return (cleanup_cd_memory(old_pwd, new_pwd, target, 1));
	if (update_pwd_env(old_pwd, new_pwd, env) == 1)
		return (cleanup_cd_memory(old_pwd, new_pwd, target,
				print_error("cd", NULL, strerror(errno), 1)));
	return (cleanup_cd_memory(old_pwd, new_pwd, target, 0));
}
