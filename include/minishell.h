/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:04:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/13 22:21:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1
# define PATH_MAX 4096
# define READLINE_MSG "minishell> "

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

typedef struct s_tools			t_tools;
typedef struct s_simple_cmds	t_simple_cmds;
typedef struct s_lexer			t_lexer;

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
int		reset_tools(t_tools *tools);
void	sigquit_handler(int sig);

/*
** Utils
*/
void	*safe_malloc(size_t size);
void	free_arr(char **arr);
void	single_cmd(t_simple_cmds *cmds, t_tools *tools);
void	executor(t_tools *tools);

// Parser definitions
typedef enum s_tokens
{
	PIPE = 1,
	GREAT,
	GREAT_GREAT,
	LESS,
	LESS_LESS
} t_tokens;

typedef struct s_global
{
	int				error_num;
	int				stop_heredoc;
	int				in_cmd;
	int				in_heredoc;
} t_global;
extern t_global		g_global;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
} t_lexer;

typedef struct s_parser_tools
{
	t_lexer			*lexer_list;
	t_lexer			*redirections;
	int				num_redirections;
	struct s_tools	*tools;
} t_parser_tools;

// Simple commands list
typedef struct s_simple_cmds
{
	char				**str;
	int					(*builtin)(struct s_tools *, struct s_simple_cmds *);
	int					num_redirections;
	char				*hd_file_name;
	t_lexer				*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
} t_simple_cmds;

// Core tools struct
typedef struct s_tools
{
	char			*args;
	char			**paths;
	char			**envp;
	t_simple_cmds	*simple_cmds;
	t_lexer			*lexer_list;
	char			*pwd;
	char			*old_pwd;
	int				pipes;
	int				*pid;
	bool			heredoc;
	bool			reset;
} t_tools;

// Parser function prototypes
int					parse_envp(t_tools *tools);
int					find_pwd(t_tools *tools);
int					parser(t_tools *tools);

// Utils function prototypes
int					count_quotes(char *line);
int					implement_tools(t_tools *tools);
char				**ft_arrdup(char **arr);
int					minishell_loop(t_tools *tools);
t_simple_cmds		*ft_simple_cmdsnew(char **str, int num_redirections,
					t_lexer *redirections);
void				ft_simple_cmdsadd_back(t_simple_cmds **lst, t_simple_cmds *new);
void				ft_simple_cmds_rm_first(t_simple_cmds **lst);
void				ft_simple_cmdsclear(t_simple_cmds **lst);
t_simple_cmds		*ft_simple_cmdsfirst(t_simple_cmds *map);

// Add prototype for builtin_arr
int					(*builtin_arr(char *str))(t_tools *tools,
					t_simple_cmds *simple_cmd);

t_lexer				*ft_lexernew(char *str, int token);
void				ft_lexeradd_back(t_lexer **lst, t_lexer *new);
void				ft_lexerdelone(t_lexer **lst, int i);
void				ft_lexerclear(t_lexer **lst);

int					token_reader(t_tools *tools);
int					add_node(char *str, t_tokens token, t_lexer **lexer_list);
t_tokens			check_token(int c);
int					handle_token(char *str, int i, t_lexer **lexer_list);
int					equal_sign(char *str);

// Error function prototypes
void				parser_error(int error, t_tools *tools, t_lexer *lexer_list);
int					parser_double_token_error(t_tools *tools, t_lexer *lexer_list,
					t_tokens token);
void				lexer_error(int error, t_tools *tools);
int					cmd_not_found(char *str);
int					export_error(char *c);
int					ft_error(int error, t_tools *tools);

// Lexer function prototype
int					handle_quotes(int i, char *str, char del);

// Expander function prototypes
size_t				dollar_sign(char *str);
int					after_dol_lenght(char *str, int j);
char				*char_to_str(char c);
int					question_mark(char **tmp);
char				*delete_quotes(char *str, char c);
int					loop_if_dollar_sign(t_tools *tools, char *str, char **tmp, int j);
int					handle_digit_after_dollar(int j, char *str);
char				*detect_dollar_sign(t_tools *tools, char *str);
char				**expander(t_tools *tools, char **str);
char				*expander_str(t_tools *tools, char *str);

// Parser utilities
t_parser_tools		init_parser_tools(t_lexer *lexer_list, t_tools *tools);
void				count_pipes(t_lexer *lexer_list, t_tools *tools);
int					count_args(t_lexer *lexer_list);
t_lexer				*find_next_cmd(t_lexer *lexer_lst);

// Handle redirections
int					add_new_redirection(t_lexer *tmp,
					t_parser_tools *parser_tools);
int					handle_heredoc(t_parser_tools *parser_tools, t_lexer *tmp);
void				rm_redirections(t_parser_tools *parser_tools);

#endif
