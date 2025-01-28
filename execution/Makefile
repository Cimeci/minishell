# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 12:50:03 by inowak--          #+#    #+#              #
#    Updated: 2025/01/28 12:58:59 by ncharbog         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = src/utils/command.c\
		src/utils/free.c\
		src/utils/size.c\
		src/parsing/commands.c\
		src/parsing/env_variables.c\
		src/parsing/main.c\
		src/parsing/parsing_utils.c\
		src/parsing/token.c\
		src/exec/main_cd.c\
		src/exec/utils_cd.c\
		src/exec/main_pwd.c\
		src/exec/main_echo.c\
		src/exec/write_utils_echo.c\
		src/exec/main_env.c\
		src/exec/main_export.c\
		src/exec/main_unset.c\
		src/exec/exec.c\
		src/exec/lst_funct.c\
		src/exec/lst_funct_utils.c\
		src/exec/main_heredoc.c\
		src/exec/signal.c\
		src/exec/randomizer.c\
		gnl/get_next_line.c\
		gnl/get_next_line_utils.c\
		# src/exec/main_exit.c\

OBJ_DIR = .objects
INCLUDE_DIR = includes
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(OBJ_LIBFT):
	@cd $(LIBFT_DIR) && make

$(NAME): $(OBJ) $(OBJ_LIBFT)
	@$(CC) $(CFLAGS) -lreadline $(OBJ) $(OBJ_LIBFT) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) -I$(INCLUDE_DIR) -O0 -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(OBJ_LIBFT)
	@cd $(LIBFT_DIR) && make clean
	@echo "clean"
fclean: clean
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re
