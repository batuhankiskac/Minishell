NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -I include -I /usr/include/readline

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

LDLIBS = -lreadline

SRCS = src/main/main.c \
	   src/main/process_line.c \
	   src/main/loop_errors.c \
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
	   src/exec/pipe_child.c \
	   src/exec/exec_redirections.c \
	   src/exec/exec_utils.c \
	   src/exec/execute.c \
	   src/exec/find_path.c \
	   src/exec/handle_heredoc.c \
	   src/exec/heredoc_utils.c \
	   src/exec/is_builtin.c \
	   src/expander/expander.c \
	   src/expander/expander_utils_1.c \
	   src/expander/expander_utils_2.c \
	   src/expander/expand_on_args.c \
	   src/expander/expand_on_redirs.c \
	   src/lexer/check_token.c \
	   src/lexer/handle_token.c \
	   src/lexer/read_words.c \
	   src/lexer/skip_spaces.c \
	   src/lexer/token_add.c \
	   src/lexer/token_reader.c \
	   src/lexer/tokenize_and_clear.c \
	   src/parser/build_command_list.c \
	   src/parser/parse_commands.c \
	   src/parser/parse_commands_block.c \
	   src/parser/parse_commands_utils.c \
	   src/parser/parse_commands_validation.c \
	   src/parser/parse_redirections.c \
	   src/signals/signal_flag.c \
	   src/signals/signal_handlers.c \
	   src/utils/utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDLIBS)

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

main: all clean

.PHONY: all clean fclean re
.SILENT: all clean fclean re
