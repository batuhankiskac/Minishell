/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:34:24 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/15 15:11:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1
# define PATH_MAX 4096

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_shell
{
	t_env	*env;
}				t_shell;

/*
** Builtins
*/
int		builtin_echo(int argc, char **args);
int		builtin_env(t_env *env);
int		builtin_export(int argc, char **args, t_env **env);
int		builtin_pwd(void);
int		builtin_unset(int argc, char **args, t_env **env);
int		builtin_exit(int argc, char **args, t_env **env);

/*
** Environment
*/
t_env	*env_init(char *envp[]);
t_env	*find_env(char *key, t_env *env);
int		env_value_len(char *str);
int		is_valid_env(char *str);
int		is_env_char(char c);
char	**env_list_to_array(t_env *env);
char	*env_value(char *env);
char	*get_env_value(char *key, t_env *env);
void	free_env(t_env *env);
void	free_env_array(char *env_array[]);
void	print_sorted_env(t_env *env);
void	sort_env(char **env_array);
void	update_env(char *key, char *value, t_env **env);

/*
** Utils
*/
void	*safe_malloc(size_t size);

#endif
