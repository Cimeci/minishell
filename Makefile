# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 12:50:03 by inowak--          #+#    #+#              #
#    Updated: 2025/02/21 16:17:21 by inowak--         ###   ########.fr        #
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

SRC = src/main.c\
		src/utils/errors.c\
		src/utils/free.c\
		src/utils/free_lst.c\
		src/utils/lst_funct.c\
		src/utils/lst_funct_utils.c\
		src/utils/utils_libft.c\
		src/utils/utils_path.c\
		src/parsing/commands/commands.c\
		src/parsing/commands/commands_redir.c\
		src/parsing/commands/commands_utils.c\
		src/parsing/init.c\
		src/parsing/parsing.c\
		src/parsing/parsing_utils.c\
		src/parsing/prompt.c\
		src/parsing/env/env_variables.c\
		src/parsing/env/env_variables_utils.c\
		src/parsing/env/expand_line.c\
		src/parsing/token/token_utils.c\
		src/parsing/token/token.c\
		src/exec/built-in/exit_utils.c\
		src/exec/built-in/export_check_print.c\
		src/exec/built-in/export_get.c\
		src/exec/built-in/export_handle.c\
		src/exec/built-in/main_cd.c\
		src/exec/built-in/main_echo.c\
		src/exec/built-in/main_env.c\
		src/exec/built-in/main_exit.c\
		src/exec/built-in/main_export.c\
		src/exec/built-in/main_pwd.c\
		src/exec/built-in/main_unset.c\
		src/exec/built-in/update_pwd.c\
		src/exec/exec_file.c\
		src/exec/exec_handle_file.c\
		src/exec/exec_built_in.c\
		src/exec/exec_handle.c\
		src/exec/exec_parent.c\
		src/exec/exec_child.c\
		src/exec/exec.c\
		src/exec/heredoc/main_heredoc.c\
		src/exec/heredoc/randomizer.c\
		src/exec/signal/signal.c\

OBJ_DIR = .objects
INCLUDE_DIR = includes
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(OBJ_LIBFT):
	@cd $(LIBFT_DIR) && make >> /dev/null

$(NAME): $(OBJ) $(OBJ_LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(OBJ_LIBFT) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -lreadline -o $(NAME)
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
	@printf "$(GREEN)clean$(RESET)\n"
fclean: clean
	@printf "$(RED)fclean$(RESET)\n"
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re