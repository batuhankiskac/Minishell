# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 16:27:35 by bkiskac           #+#    #+#              #
#    Updated: 2025/03/15 16:53:25 by bkiskac          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = src/main.c \
	   src/env/env_init.c \
	   src/env/env_print.c \
	   src/env/env_to_array.c \
	   src/env/env_update.c \
	   src/env/env_utils.c \
	   src/env/env_value.c \
	   src/builtins/cd.c \
	   src/builtins/echo.c \
	   src/builtins/env.c \
	   src/builtins/exit.c \
	   src/builtins/export.c \
	   src/builtins/pwd.c \
	   src/builtins/unset.c \
	   src/utils/utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
