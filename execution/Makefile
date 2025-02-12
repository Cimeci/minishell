# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: noacharbogne <noacharbogne@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 12:50:03 by inowak--          #+#    #+#              #
#    Updated: 2025/02/12 15:40:26 by noacharbogn      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

########################
#        Color         #
########################

RED    = \033[1;31m
GREEN  = \033[1;32m
YELLOW = \033[1;33m
BLUE   = \033[1;34m
CYAN   = \033[1;36m
PURPLE = \033[1;35m
SPRUCE_GREEN = \033[38;2;0;102;51m
KING_BLUE = \033[38;2;65;105;225m
RESET  = \033[0m

CC = cc

CFLAGS = -Wall -Werror -Wextra -g -I/usr/local/opt/readline/include

NAME = minishell

SRC = src/utils/command.c\
		src/utils/free.c\
		src/utils/size.c\
		src/utils/errors.c\
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
		src/exec/main_exit.c\

OBJ_DIR = .objects
INCLUDE_DIR = includes
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(OBJ_LIBFT):
	@cd $(LIBFT_DIR) && make > /dev/null

$(NAME): $(OBJ) $(OBJ_LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_LIBFT) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -lreadline -o $(NAME)
	@printf "$(PURPLE)            _       _     _          _ _ \n"
	@printf "$(PURPLE)           (_)     (_)   | |        | | |\n"
	@printf "$(PURPLE)  _ __ ___  _ _ __  _ ___| |__   ___| | |\n"
	@printf "$(KING_BLUE) | '_ \` _ \| | '_ \| / __| '_ \ / _ \ | |\n"
	@printf "$(KING_BLUE) | | | | | | | | | | \__ \ | | |  __/ | |\n"
	@printf "$(KING_BLUE) |_| |_| |_|_|_| |_|_|___/_| |_|\___|_|_|\n"
	@printf "$(RESET)\n"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) -I$(INCLUDE_DIR) -O0 -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(OBJ_LIBFT)
	@cd $(LIBFT_DIR) && make clean > /dev/null
	@echo "clean"
fclean: clean
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re
