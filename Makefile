# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 16:27:35 by bkiskac           #+#    #+#              #
#    Updated: 2025/04/20 17:52:49 by bkiskac          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -I include

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = src/main.c \
	   src/builtins/cd.c \
	   src/builtins/echo.c \
	   src/builtins/env.c \
	   src/builtins/exit.c \
	   src/builtins/export.c \
	   src/builtins/pwd.c \
	   src/builtins/unset.c \
	   src/env/env_init.c \
	   src/env/env_print.c \
	   src/env/env_to_array.c \
	   src/env/env_update.c \
	   src/env/env_utils.c \
	   src/env/env_value.c \
	   src/exec/exec_builtin.c \
	   src/exec/exec_external.c \
	   src/exec/exec_pipe.c \
	   src/exec/exec_redirections.c \
	   src/exec/exec_utils.c \
	   src/exec/execute.c \
	   src/exec/handle_heredoc.c \
	   src/exec/is_builtin.c \
	   src/signals/signals.c \
	   src/utils/utils.c \
	   src/expander/expander.c \
	   src/expander/expanders_utils.c \
	   src/expander/expanders_utils2.c \
	   src/error/error_handling.c \
	   src/error/ft_error.c \
	   src/lexer/handle_quotes.c \
	   src/lexer/handle_token.c \
	   src/lexer/token_reader.c \
	   src/parser/handle_redirections.c \
	   src/parser/parser_utils.c \
	   src/parser/parser.c \
	   src/parser_utils/minishell_loop.c \
	   src/parser_utils/parse_envp.c \
	   src/parser_utils/t_lexer_clear_utils.c \
	   src/parser_utils/t_lexer_utils.c \
	   src/parser_utils/t_simple_cmds_utils_1.c \
	   src/parser_utils/t_simple_cmds_utils_2.c \
	   src/parser_utils/utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

main: all clean

.PHONY: all clean fclean re main
