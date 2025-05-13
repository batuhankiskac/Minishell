/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/15 16:10:24 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * get_target - Determines the target directory for the cd command.
 *
 * This function fetches the HOME environment variable
 * when no argument is provided,
 * or duplicates the provided directory argument otherwise.
 *
 * Parameters:
 *   argc - The argument count.
 *   args - Array of command arguments.
 *   env - Pointer to the environment variables linked list.
 *
 * Returns:
 *   A pointer to the target directory string, or NULL if an error occurs.
 */
static char	*get_target(int argc, char **args, t_env **env)
{
	char	*target;

	if (argc < 2)
	{
		target = get_env_value("HOME", *env);
		if (!target)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (free(target), NULL);
		}
	}
	else
	{
		target = ft_strdup(args[1]);
		if (!target)
			return (perror("cd error"), NULL);
	}
	return (target);
}

/*
 * change_directory - Changes the current working
 * directory and retrieves the new path.
 *
 * Parameters:
 *   target - The directory to switch to.
 *   new_pwd - Buffer to store the new current working directory.
 *
 * Returns:
 *   0 on success, or ERROR if the operation fails.
 */
static int	change_directory(char *target, char *new_pwd)
{
	if (chdir(target) == -1)
	{
		perror("cd error");
		return (ERROR);
	}
	if (!getcwd(new_pwd, PATH_MAX))
	{
		perror("getcwd error");
		return (ERROR);
	}
	return (0);
}

/*
 * builtin_cd - Executes the cd builtin command.
 *
 * This function changes the current working directory,
 * updates the environment variables
 * OLD_PWD and PWD accordingly.
 *
 * Parameters:
 *   argc - The argument count.
 *   args - An array of strings containing command arguments.
 *   env - Pointer to the head of the environment variables linked list.
 *
 * Returns:
 *   0 on success, or ERROR upon failure.
 */
int	builtin_cd(int argc, char **args, t_env **env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	char	*target;

	if (!getcwd(old_pwd, PATH_MAX))
		return (perror("getcwd error"), ERROR);
	target = get_target(argc, args, env);
	if (!target)
		return (ERROR);
	if (change_directory(target, new_pwd) == ERROR)
		return (free(target), ERROR);
	update_env("OLDPWD", old_pwd, env);
	update_env("PWD", new_pwd, env);
	free(target);
	return (0);
}
