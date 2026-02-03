# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/14 14:31:49 by rgregori          #+#    #+#              #
#    Updated: 2025/08/29 13:51:38 by rgregori         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Diretórios
SRC_MANDATORY_DIR := src/mandatory
SRC_BONUS_DIR     := src/bonus
OBJ_MANDATORY_DIR := obj/mandatory
OBJ_BONUS_DIR     := obj/bonus
INCLUDE_DIR       := include

# Compilador e flags
CC       := cc
CFLAGS   := -Wall -Wextra -Werror -I$(INCLUDE_DIR)

# Biblioteca
NAME     := libft.a
AR       := ar
ARFLAGS  := rcs

# Fontes mandatory
SRCS_MANDATORY := ft_atoi.c ft_bzero.c ft_calloc.c ft_memchr.c ft_isalpha.c \
                  ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c \
                  ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c \
                  ft_strnstr.c ft_strrchr.c ft_isalnum.c ft_isdigit.c ft_substr.c \
                  ft_isprint.c ft_split.c ft_strjoin.c ft_tolower.c ft_isascii.c \
                  ft_strtrim.c ft_toupper.c ft_itoa.c ft_strmapi.c ft_striteri.c \
                  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
				  ft_strcpy.c ft_atod.c

OBJS_MANDATORY := $(addprefix $(OBJ_MANDATORY_DIR)/,$(SRCS_MANDATORY:.c=.o))

# Fontes bonus
SRCS_BONUS := ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c \
              ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c \
              ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c

OBJS_BONUS := $(addprefix $(OBJ_BONUS_DIR)/,$(SRCS_BONUS:.c=.o))

# Alvos
.PHONY: all clean fclean re bonus full

all: $(NAME)

$(NAME): $(OBJS_MANDATORY)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS_MANDATORY)

bonus: $(OBJS_MANDATORY) $(OBJS_BONUS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS_BONUS)

full: all bonus

$(OBJ_MANDATORY_DIR)/%.o: $(SRC_MANDATORY_DIR)/%.c | $(OBJ_MANDATORY_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_MANDATORY_DIR):
	mkdir -p $@

$(OBJ_BONUS_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_MANDATORY_DIR) $(OBJ_BONUS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
