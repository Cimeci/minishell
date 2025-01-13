# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 12:50:03 by inowak--          #+#    #+#              #
#    Updated: 2025/01/13 15:08:48 by inowak--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = command.c\
		lst_funct_utils.c\
		main_echo.c\
		main_export.c\
		utils_cd.c\
		free.c\
		main.c\
		main_env.c\
		main_pwd.c\
		write_utils_echo.c\
		lst_funct.c\
		main_cd.c\
		main_exit.c\
		main_unset.c

OBJ_DIR = .objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = ../libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)
	@echo "compiled"

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