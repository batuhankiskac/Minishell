/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:04:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 22:11:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1
# define PATH_MAX 4096

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

typedef enum e_redir_type
{
	REDIR_NONE = 0,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}			t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	int					argc;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	t_env		*env;
	t_command	*command;
	int			exit_status;
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
int		builtin_cd(int argc, char **args, t_env **env);

/*
** Environment
*/
t_env	*env_init(char *envp[]);
t_env	*find_env(char *key, t_env *env);
int		env_value_len(char *str);
int		is_valid_env(char *str);
int		is_env_char(char c);
int		is_valid_identifier(char *str);
char	**env_list_to_array(t_env *env);
char	*env_value(char *env);
char	*get_env_value(char *key, t_env *env);
void	free_env(t_env *env);
void	print_sorted_env(t_env *env);
void	sort_env(char **env_array);
void	update_env(char *key, char *value, t_env **env);

/*
** Execution
*/
int		is_builtin(char *cmd);
int		exec_builtin(t_shell *shell);
int		exec_external(t_shell *shell);
int		open_file(char *filename, int flags, int mode, char *type);
int		dup_fd(int old_fd, int new_fd, char *type);
int		setup_redir(t_shell *shell);
int		handle_heredoc_redir(t_redir *redir);
int		execute_pipe(t_shell *shell);
char	*find_path(char *cmd, char *envp[]);
char	*get_env(char *name, char *envp[]);
void	run_command(t_shell *shell);
void	close_pipe_fd(int prev_fd, int pipe_fd[2]);

/*
** Signals
*/
void	init_signals(void);
void	reset_signals(void);
void	sigquit_handler(int sig);

/*
** Utils
*/
void	*safe_malloc(size_t size);
void	ft_free_all(char **arr);

#endif
