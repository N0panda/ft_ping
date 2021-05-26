# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/22 16:36:01 by ythomas           #+#    #+#              #
#    Updated: 2021/05/25 18:13:10 by ythomas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_ping
CC			= gcc

CFLAGS		= -Wall -Wextra -Werror

SRCS_DIR	= srcs/
H_DIR		= includes/
O_DIR		= objs/
$(shell mkdir -p $(O_DIR))

INCS		= $(wildcard $(H_DIR)*.h)
SRCS		= $(wildcard $(SRCS_DIR)*.c)
OBJS		= $(SRCS:$(SRCS_DIR)%.c=$(O_DIR)%.o)

HEADERS		= -I./$(H_DIR)


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm $(HEADERS)

$(O_DIR)%.o: $(SRCS_DIR)%.c $(INCS)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re