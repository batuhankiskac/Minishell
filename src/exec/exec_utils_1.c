/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:53:36 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 16:40:15 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
* get_env - Retrieves the value of an environment variable from the envp array.
*
* This function searches for an environment variable in the envp array
* and returns its value if found. The variable name is expected to be
* in the format "NAME=VALUE".
*
* Parameters:
*   char *name - The name of the environment variable to search for.
*   char **envp - The array of environment variables.
*
* Returns:
*   A pointer to the value of the environment variable, or NULL if not found.
*/
char	*get_env(char *name, char *envp[])
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/*
* find_path - Searches for the full path of a command
* in the PATH environment variable.
*
* This function splits the PATH environment variable into its components
* and checks each component to see if the command exists and is executable.
* If found, it returns the full path to the command; otherwise, it returns
* the original command string.
*
* Parameters:
*   char *cmd - The command to search for.
*   char **envp - The array of environment variables.
*
* Returns:
*   A pointer to the full path of the command if found, or the original
*   command string if not found.
*/
char	*find_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	allpath = ft_split(get_env("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	i = -1;
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_all(allpath);
			ft_free_all(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(allpath);
	ft_free_all(s_cmd);
	return (cmd);
}

/*
* open_file - Opens a file with the specified flags and mode.
*
* This function attempts to open a file with the given flags and mode.
* If the file cannot be opened, it prints an error message to stderr
* and returns ERROR.
*
* Parameters:
*   char *filename - The name of the file to open.
*   int flags - The flags to use when opening the file.
*   int mode - The mode to use when creating the file (if applicable).
*   char *type - A string indicating the type of operation (for error messages).
*
* Returns:
*   The file descriptor if successful, or ERROR if an error occurs.
*/
int	open_file(char *filename, int flags, int mode, char *type)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: open ", 2);
		ft_putstr_fd(type, 2);
		ft_putstr_fd(" file", 2);
		perror(filename);
		return (ERROR);
	}
	return (fd);
}

/*
* dup_fd - Duplicates a file descriptor to a new file descriptor.
*
* This function duplicates the old file descriptor to the new file descriptor
* using dup2. If an error occurs, it prints an error message to stderr,
* closes the old file descriptor, and returns ERROR.
*
* Parameters:
*   int old_fd - The old file descriptor to duplicate.
*   int new_fd - The new file descriptor to duplicate to.
*   char *type - A string indicating the type of operation (for error messages).
*
* Returns:
*   0 if successful, or ERROR if an error occurs.
*/
int	dup_fd(int old_fd, int new_fd, char *type)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		ft_putstr_fd("minishell: dup2 ", 2);
		perror(type);
		close(old_fd);
		return (ERROR);
	}
	close(old_fd);
	return (0);
}
