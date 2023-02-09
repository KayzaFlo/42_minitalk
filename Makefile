# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 13:26:56 by fgeslin           #+#    #+#              #
#    Updated: 2023/02/09 12:21:40 by fgeslin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#####	CONFIG		############################################################
SERVER		:= server
CLIENT		:= client
LIBFT		:= libft/libft.a
CC			:= gcc
AR			:= ar -rcs
RM			:= rm -f
CFLAGS		= -Wall -Wextra -Werror

#####	SOURCES		############################################################
SRC_SERVER	=	server.c
SRC_CLIENT	=	client.c
SRC_BONUS	=	
OBJ_SERVER	= $(SRC_SERVER:.c=.o)
OBJ_CLIENT	= $(SRC_CLIENT:.c=.o)
OBJ_BONUS	= $(SRC_BONUS:.c=.o)

#####	Makefile  objs		###################################################
WHITE		:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
PINK		:= \033[1;35m
CYAN 		:= \033[1;36m

.c.o:
	@ echo "$(YELLOW)Compiling: $(WHITE)$<"
	@ ${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all: $(LIBFT) $(SERVER) $(CLIENT)

$(SERVER): $(OBJ_SERVER)
	@ echo "$(GREEN)Compilation ${WHITE}of ${CYAN}$(SERVER) ${WHITE}..."
	@ $(CC) -o $(SERVER) $(OBJ_SERVER) $(LIBFT) -lm
	@ echo "$(CYAN)$(SERVER) $(GREEN)created$(WHITE) ✔️"

$(CLIENT): $(OBJ_CLIENT)
	@ echo "$(GREEN)Compilation ${WHITE}of ${CYAN}$(CLIENT) ${WHITE}..."
	@ $(CC) -o $(CLIENT) $(OBJ_CLIENT) $(LIBFT) -lm
	@ echo "$(CYAN)$(CLIENT) $(GREEN)created$(WHITE) ✔️"

bonus: $(LIBFT) $(OBJ_BONUS)
	@ echo "$(GREEN)Compilation ${WHITE}of ${CYAN}$(NAME) ${WHITE}..."
	# @ $(CC) -o $(BONUS) $(OBJ_BONUS) $(LIBFT) -lm
	@ echo "$(CYAN)$(CHECKER) $(GREEN)created$(WHITE) ✔️"

$(LIBFT):
	@ echo "$(GREEN)Compilation ${WHITE}of ${CYAN}${LIBFT} ${WHITE}..."
	@ $(MAKE) -C libft/
	@ echo "$(CYAN)$(LIBFT) $(GREEN)created$(WHITE) ✔️"

clean:
	@ ${RM} $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_BONUS)
	@ $(MAKE) clean -C libft/
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(WHITE)and $(CYAN)$(LIBFT) $(WHITE)objs ✔️"

fclean: clean
	@ ${RM} $(SERVER) $(CLIENT)
	@ $(MAKE) fclean -C libft/
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(WHITE)and $(CYAN)$(LIBFT) $(WHITE)binary ✔️"

re: fclean all

rebonus: fclean bonus

.PHONY:		all clean fclean re
