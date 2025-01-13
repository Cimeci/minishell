# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/11 08:15:54 by ncharbog          #+#    #+#              #
#    Updated: 2025/01/13 11:38:39 by inowak--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = main.c \
		command.c \
		free.c \
		lst_funct_utils.c \
		lst_funct.c \

OBJ_DIR = .objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = ../libft
OBJ_LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(OBJ_LIBFT):
	cd $(LIBFT_DIR) && make

$(NAME): $(OBJ) $(OBJ_LIBFT)
	$(CC) $(CFLAGS) -lreadline $(OBJ) $(OBJ_LIBFT) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -O0 -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(OBJ_LIBFT)
	cd $(LIBFT_DIR) && make clean
fclean: clean
	rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re