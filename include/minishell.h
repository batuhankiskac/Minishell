/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:34:24 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/19 21:58:39 by bkiskac          ###   ########.fr       */
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

/*
* t_redir_type - Enumeration for different types of redirection in Minishell.
*
* This enumeration defines the various types of redirection that can be
* performed in the shell. Each type corresponds to a specific operation
* that can be performed on file descriptors.
*
* Values:
*   REDIR_NONE   - No redirection.
*   REDIR_IN     - Input redirection (e.g., < file).
*   REDIR_OUT    - Output redirection (e.g., > file).
*   REDIR_APPEND - Append redirection (e.g., >> file).
*   REDIR_HEREDOC - Heredoc redirection (e.g., << delimiter).
*/
typedef enum e_redir_type
{
	REDIR_NONE = 0,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}			t_redir_type;

/*
* t_redir - Structure for representing a redirection in Minishell.
*
* This structure defines a node used in a linked list that stores
* information about redirections. Each node contains the type of
* redirection, the file associated with the redirection, and a pointer
* to the next node in the list.
*
* Fields:
*   type - The type of redirection (input, output, append, heredoc).
*   file - The file associated with the redirection.
*   next - A pointer to the next t_redir node in the linked list.
*/
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

/*
* t_env - Structure for representing an environment variable in Minishell.
*
* This structure defines a node used in a linked list that stores environment
* variables. Each node contains a key-value pair and a pointer to the next node
* in the list.
*
* Fields:
*   key   - The name of the environment variable.
*   value - The value assigned to the environment variable.
*   next  - A pointer to the next t_env node in the linked list.
*/
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/*
* t_command - Structure for representing a command in Minishell.
*
* This structure defines a command that can be executed in the shell.
* It contains the command string, an array of arguments, the number of
* arguments, and a pointer to a linked list of redirections associated
* with the command.
*
* Fields:
*   cmd    - The command string (e.g., "ls").
*   args   - An array of arguments for the command (e.g., ["-l", "-a"]).
*   argc   - The number of arguments in the args array.
*   redir  - A pointer to a linked list of redirections associated
*            with the command.
*/
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
** Execute
*/
int		is_builtin(char *cmd);
int		exec_builtin(t_shell *shell);
int		exec_external(t_shell *shell);
int		open_file(char *filename, int flags, int mode, char *type);
int		dup_fd(int old_fd, int new_fd, char *type);
int		setup_redir(t_shell *shell);
int		handle_heredoc_redir(t_redir *redir);
int		execute_pipe(t_shell *shell);
int		execute_pipe(t_shell *shell);
char	*find_path(char *cmd, char *envp[]);
char	*get_env(char *name, char *envp[]);
void	run_command(t_shell *shell);
void	close_pipe_fd(int prev_fd, int pipe_fd[2]);

/*
** Utils
*/
void	*safe_malloc(size_t size);
void	ft_free_all(char **arr);

#endif
