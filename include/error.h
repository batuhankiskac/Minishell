#ifndef ERROR_H
# define ERROR_H
# include "minishell.h"
# include "parser.h"

void	parser_error(int error, t_tools *tools, t_lexer *lexer_list);
int		parser_double_token_error(t_tools *tools, t_lexer *lexer_list,
			t_tokens token);
void	lexer_error(int error, t_tools *tools);
int		cmd_not_found(char *str);
int		export_error(char *c);

//ft_error
int		ft_error(int error, t_tools *tools);

#endif