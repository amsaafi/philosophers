# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 19:49:54 by samsaafi          #+#    #+#              #
#    Updated: 2024/11/18 20:06:57 by samsaafi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

Compiler = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo

INC = parse.c \
		philo.c \
		utils.c \
		


OBJ = $(INC:.c=.o)

%.o: %.c
	$(Compiler) $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
