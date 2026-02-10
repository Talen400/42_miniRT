# ******************************************************************************
# miniRT Makefile - Compilação do projeto com libft e MLX42
# ******************************************************************************

# Diretórios
SRC_DIR           := src
OBJ_DIR           := obj
BIN_DIR           := bin
INCLUDE_DIR       := include
LIBFT_DIR         := libft
MLX42_DIR         := MLX42
MLX42_BUILD_DIR   := $(MLX42_DIR)/build

# Compilador e flags
CC                := cc
CFLAGS            := -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include -I$(MLX42_DIR)/include
LDFLAGS           := -L$(LIBFT_DIR) -L$(MLX42_BUILD_DIR) -lft -lmlx42 -lm -lglfw
CFLAGS_DEBUG      := $(CFLAGS) -g -O0

# Executable
NAME              := miniRT

# Fontes do projeto
SRCS              := src/main.c \
					 src/utils.c \
					 src/scene/scene.c \
					 src/scene/object_utils.c \
					 src/parser/parser_ambient.c \
					 src/parser/parser_camera.c \
					 src/parser/parser_cylinder.c \
					 src/parser/parser_light.c \
					 src/parser/parser_plane.c \
					 src/parser/parser_sphere.c \
					 src/parser/parser_error.c \
                     src/parser/parser.c \
                     src/parser/parser_utils.c \
					 src/vectors/vec_basic.c \
					 src/vectors/vec_advanced.c \
					 src/mlx_utils.c \
					 src/draw/draw.c \
					 src/lighting/lighting.c \
					 src/color/color_convert.c \
					 src/color/color_operations.c \
					 src/ray/ray_utils.c

OBJS              := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Bibliotecas
LIBFT             := $(LIBFT_DIR)/libft.a
MLX42_LIB         := $(MLX42_BUILD_DIR)/libmlx42.a

# Alvos
.PHONY: all clean fclean re debug libft mlx42 libft_clean mlx42_clean

all: $(LIBFT) $(MLX42_LIB) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "✓ $(NAME) compilado com sucesso!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(MLX42_LIB):
	@mkdir -p $(MLX42_BUILD_DIR)
	@cd $(MLX42_BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1 || true
	@$(MAKE) -C $(MLX42_BUILD_DIR) > /dev/null 2>&1 || true

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || true

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	rm -rf $(MLX42_BUILD_DIR)

re: fclean all

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: fclean $(NAME)
	@echo "✓ Debug build concluído!"

libft:
	@$(MAKE) -C $(LIBFT_DIR)

mlx42: $(MLX42_LIB)

libft_clean:
	@$(MAKE) -C $(LIBFT_DIR) clean

libft_fclean:
	@$(MAKE) -C $(LIBFT_DIR) fclean

mlx42_clean:
	rm -rf $(MLX42_BUILD_DIR)

# Informações
info:
	@echo "Projeto: miniRT"
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Arquivos fonte: $(words $(SRCS))"
	@echo "Bibliotecas: libft MLX42"
