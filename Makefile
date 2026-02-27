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
CFLAGS            := -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include -I$(MLX42_DIR)/include
LDFLAGS           := -L$(LIBFT_DIR) -L$(MLX42_BUILD_DIR) -lft -lmlx42 -lm -lglfw
CFLAGS_DEBUG      := $(CFLAGS) -g -O0

# Executable
NAME              := miniRT

# Fontes do projeto
SRCS              := src/main.c \
					 src/utils.c \
					 src/events.c \
					 src/scene/scene.c \
					 src/scene/object_utils.c \
					 src/parser/parser_ambient.c \
					 src/parser/parser_camera.c \
					 src/parser/parser_camera_utils.c \
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
					 src/intersect/hit_sphere.c \
					 src/intersect/hit_plane.c \
					 src/intersect/hit_cylinder.c \
					 src/intersect/hit_scene.c \
					 src/intersect/hit_record.c \
					 src/lighting/lighting.c \
					 src/color/color_convert.c \
					 src/color/color_operations.c \
					 src/ray/ray_utils.c

OBJS              := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Bonus
SRC_BONUS_DIR     := src_bonus
OBJ_BONUS_DIR     := obj_bonus
INCLUDE_BONUS_DIR := include_bonus
NAME_BONUS        := miniRT_bonus

SRCS_BONUS        := src_bonus/main.c \
					 src_bonus/utils.c \
					 src_bonus/events.c \
					 src_bonus/mlx_utils.c \
					 src_bonus/scene/scene.c \
					 src_bonus/scene/object_utils.c \
					 src_bonus/scene/object_extra_args.c \
					 src_bonus/parser/parser_ambient.c \
					 src_bonus/parser/parser_camera.c \
					 src_bonus/parser/parser_camera_utils.c \
					 src_bonus/parser/parser_cylinder.c \
					 src_bonus/parser/parser_light.c \
					 src_bonus/parser/parser_plane.c \
					 src_bonus/parser/parser_sphere.c \
					 src_bonus/parser/parser_cone.c \
					 src_bonus/parser/parser_error.c \
					 src_bonus/parser/parser.c \
					 src_bonus/parser/parser_utils.c \
					 src_bonus/vectors/vec_basic.c \
					 src_bonus/vectors/vec_advanced.c \
					 src_bonus/draw/draw.c \
					 src_bonus/draw/draw_utils.c \
					 src_bonus/intersect/hit_sphere.c \
					 src_bonus/intersect/bump_mapping.c \
					 src_bonus/intersect/checkerboard.c \
					 src_bonus/intersect/hit_cone.c \
					 src_bonus/intersect/hit_plane.c \
					 src_bonus/intersect/hit_cylinder.c \
					 src_bonus/intersect/hit_scene.c \
					 src_bonus/intersect/hit_record.c \
					 src_bonus/intersect/intersect_utils.c \
					 src_bonus/lighting/lighting.c \
					 src_bonus/color/color_convert.c \
					 src_bonus/color/color_operations.c \
					 src_bonus/ray/ray_utils.c

OBJS_BONUS        := $(patsubst $(SRC_BONUS_DIR)/%.c,$(OBJ_BONUS_DIR)/%.o,$(SRCS_BONUS))
CFLAGS_BONUS      := -Wall -Wextra -Werror -I$(INCLUDE_BONUS_DIR) -I$(LIBFT_DIR)/include -I$(MLX42_DIR)/include

# Bibliotecas
LIBFT             := $(LIBFT_DIR)/libft.a
MLX42_LIB         := $(MLX42_BUILD_DIR)/libmlx42.a

# Alvos
.PHONY: all clean fclean re debug bonus libft mlx42 libft_clean mlx42_clean

all: $(LIBFT) $(MLX42_LIB) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "✓ $(NAME) compilado com sucesso!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

# Bonus rules
bonus: $(LIBFT) $(MLX42_LIB) $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS_BONUS) $(OBJS_BONUS) $(LDFLAGS) -o $(NAME_BONUS)
	@echo "✓ $(NAME_BONUS) (bonus) compilado com sucesso!"

$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c | $(OBJ_BONUS_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_BONUS) -c $< -o $@

$(OBJ_BONUS_DIR):
	mkdir -p $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(MLX42_LIB):
	@mkdir -p $(MLX42_BUILD_DIR)
	@cd $(MLX42_BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1 || true
	@$(MAKE) -C $(MLX42_BUILD_DIR) > /dev/null 2>&1 || true

clean:
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || true

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	rm -rf $(MLX42_BUILD_DIR)

re: fclean all
re_bonus : fclean bonus
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
