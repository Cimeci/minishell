# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 12:50:03 by inowak--          #+#    #+#              #
#    Updated: 2025/01/21 16:58:31 by ncharbog         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = src/utils/command.c\
		src/utils/free.c\
		src/utils/size.c\
		src/built-in/utils_cd.c\
		src/built-in/main_cd.c\
		src/parsing/commands.c\
		src/parsing/env_variables.c\
		src/parsing/main.c\
		src/parsing/parsing_utils.c\
		src/parsing/token.c\
		src/built-in/main_pwd.c\
		src/built-in/main_echo.c\
		src/built-in/write_utils_echo.c\
		src/built-in/main_env.c\
		src/built-in/main_export.c\
		src/built-in/lst_funct.c\
		src/built-in/lst_funct_utils.c\
		# src/built-in/main.c\
		# src/built-in/main_exit.c\
		# src/built-in/main_unset.c\

OBJ_DIR = .objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = ../libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(OBJ_LIBFT):
	@cd $(LIBFT_DIR) && make

$(NAME): $(OBJ) $(OBJ_LIBFT)
	@$(CC) $(CFLAGS) -lreadline $(OBJ) $(OBJ_LIBFT) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) -O0 -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(OBJ_LIBFT)
	@cd $(LIBFT_DIR) && make clean
	@echo "clean"
fclean: clean
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re