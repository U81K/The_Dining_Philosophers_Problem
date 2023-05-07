# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/02 14:36:53 by bgannoun          #+#    #+#              #
#    Updated: 2023/05/07 14:21:09 by bgannoun         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo_bonus
CFLAGS	=	-Wall -Wextra -Werror -pthread
RM		=	rm -rf
OBJ		=	$(SRC:.c=.o)

SRC		=	main_bonus.c \
			src_bonus.c \
			create_pro.c \


$(NAME)	:	$(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all		:	$(NAME)

clean	:	
		$(RM) $(OBJ)

fclean	:	clean
		$(RM) $(NAME)

re		:	fclean all