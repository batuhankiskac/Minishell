/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/16 20:59:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# define ERROR -1

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}			t_redir_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*str;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	char			*original_file;
	int				expand_content;
	int				heredoc_fd;
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
	t_token		*tokens;
	t_redir		*redir;
	char		*line;
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
int		builtin_exit(t_shell *shell);
int		builtin_cd(int argc, char **args, t_env **env);

/*
** Environment
*/
t_env	*env_init(char *envp[]);
t_env	*find_env(char *key, t_env *env);
int		is_env_char(char c);
int		is_valid_identifier(char *str);
int		update_env(char *key, char *value, t_env **env);
char	*env_value(char *env);
char	*get_env_value(char *key, t_env *env);
char	**env_list_to_array(t_env *env);
char	*find_in_envp(char *name, char *envp[]);
void	free_env(t_env *env);
void	print_sorted_env(t_env *env);
void	sort_env(char **env_array);

/*
** Execution
*/
int		is_builtin(char *cmd);
int		exec_builtin(t_shell *shell);
int		exec_external(t_shell *shell);
int		open_file(char *filename, int flags, int mode);
int		dup_fd(int old_fd, int new_fd, char *type);
int		setup_redir(t_shell *shell);
int		handle_heredoc_redir(t_shell *shell);
int		execute_pipe(t_shell *shell);
int		apply_redirection(t_redir *redir);
int		validate_command(t_shell *shell, char **env_array);
char	*find_path(char *cmd, char *envp[]);
void	exec_external_direct(t_shell *shell, char **env_array);
void	find_and_exec_command(t_shell *shell, char **env_array);
void	run_command(t_shell *shell);
void	close_pipe_fd(int prev_fd, int pipe_fd[2]);
void	pipe_child_process(t_shell *shell,
			t_command *cmd, int prev_fd, int pipe_fd[2]);

/*
** Expander
*/
int		expand_on_args(t_command *cmd, t_env *env, int exit_status);
int		expand_on_redirs(t_command *cmd, t_env *env, int exit_status);
int		expander(t_shell *shell);
char	*append_variable(char *res, char *s, int *i, t_shell *shell);
char	*append_literal(char *res, const char *s, int start, int end);
char	*expand_string(const char *s, t_env *env, int exit_status);
char	*expand_text_chunk(char *res, const char *s, int *i);
char	*handle_single_quote(char *res, const char *s, int *i);
char	*handle_double_quote(char *res, const char *s, int *i,
			t_shell *shell_context);

/*
** Lexer
*/
int		check_token(char c);
int		handle_token(const char *str, int pos, t_token **tokens);
int		read_words(int pos, const char *str, t_token **tokens);
int		tokenize_line(char *line, t_shell *shell);
int		token_add(t_token **tokens, const char *word, int len);
void	clear_command_list(t_command *cmd);
void	clear_token_list(t_token **tokens_head);

/*
** Main
*/
int		process_line(char *raw_line_ptr, t_shell *shell);

/*
** Parser
*/
int		parse_redirections(t_shell *shell);
int		build_command_list(t_shell *shell);
int		parse_commands(t_shell *shell);
int		process_command_block(t_command *cmd, t_token **t_ptr);
int		count_words_until_pipe(t_token *t);
int		init_command_args(t_command *cmd, int count);
int		validate_pipe_sequence(t_token *tokens);
int		set_heredoc_properties(t_redir *r, const char *file);
int		set_redir_file(t_redir *r, const char *file);
void	populate_args(t_token **t_ptr, t_command *cmd);
void	set_command_name(t_command *cmd);

/*
** Signal Handling
*/
int		get_signal_flag(void);
void	set_interactive_signals(void);
void	reset_signals(void);
void	set_signal_flag(int sig);
void	reset_signal_flag(void);
void	set_exec_signals(void);

/*
** Utils
*/
int		print_error(char *cmd, char *arg, char *msg, int err_code);
int		cleanup_cd_memory(char *old_pwd, char *new_pwd, char *target,
			int return_value);
int		handle_wait_status(int status, char **env_array);
void	ft_free_all(char **arr);
void	free_redirections(t_redir *r);
void	*print_error_null(char *cmd, char *arg, char *msg);
void	cleanup_iteration_resources(char *raw_line_ptr, t_shell *shell);
void	cleanup_child_process(t_shell *shell, char **env_array);
void	cleanup_child_and_exit(t_shell *shell, char **env_array,
			t_command *original_head, int exit_code);

#endif
