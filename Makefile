# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/07 12:03:11 by amugnier          #+#    #+#              #
#    Updated: 2024/02/06 18:21:39 by casomarr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



# **************************************************************************** #
#                                  CONFIGURATION                               #
# **************************************************************************** #

NAME = minirt

CC = clang
#caro : rajouter -gdwarf-4 qd sur ordi perso pour valgrind
CFLAGS = -Wall -Wextra -g3 -gdwarf-4
#/!\ ATTENTION, FAUT ENLEVER LE # DE LA PROCHAINE LIGNE AVANT LE RENDU /!\
#CFLAGS+= -Werror
LINKER_FLAGS = -lXext -lX11 -lm #check location installation of dependencies to github action

# LX11 = /usr/lib/x86_64-linux-gnu/libX11.a
# LXEXT = /usr/lib/x86_64-linux-gnu/libXext.a
# LM = /usr/lib/x86_64-linux-gnu/libm.a

# LINKER_FLAGS = $(LX11) $(LXEXT) $(LM)

PATH_LIBFT = libft
PATH_INCLUDE = includes
PATH_SRCS = srcs
PATH_MLX = mlx

RM = rm -f

# **************************************************************************** #
#                                  DEPENDANCIES                                #
# **************************************************************************** #

### LIBRAIRIES
LIBFT = $(PATH_LIBFT)/libft.a

MLX = $(PATH_MLX)/libmlx.a

### HEADERS
HDRS = $(PATH_INCLUDE)/minirt.h \

### SOURCES
SRCS = $(PATH_SRCS)/check_objs.c \
	   $(PATH_SRCS)/check_data_objs.c \
	   $(PATH_SRCS)/add_substract_maths.c \
	   $(PATH_SRCS)/color.c \
	   $(PATH_SRCS)/compare_maths.c \
	   $(PATH_SRCS)/cylinder.c \
	   $(PATH_SRCS)/dot_maths.c \
	   $(PATH_SRCS)/intersections.c \
	   $(PATH_SRCS)/lightray.c \
	   $(PATH_SRCS)/main.c \
	   $(PATH_SRCS)/multiply_divide_maths.c \
	   $(PATH_SRCS)/norm_maths.c \
	   $(PATH_SRCS)/open_file.c \
	   $(PATH_SRCS)/parse_file.c \
	   $(PATH_SRCS)/plane.c \
	   $(PATH_SRCS)/rays.c \
	   $(PATH_SRCS)/render.c \
	   $(PATH_SRCS)/sphere.c \
	   $(PATH_SRCS)/sqrt_maths.c \
	   $(PATH_SRCS)/utils.c \
	   $(PATH_SRCS)/init.c \
	   $(PATH_SRCS)/check_params.c \
	   $(PATH_SRCS)/set_data.c \
	   $(PATH_SRCS)/write_scene_ppm.c

SRCS_PARSING = $(PATH_SRCS)/check_objs.c \
	   $(PATH_SRCS)/check_data_objs.c \
	   $(PATH_SRCS)/open_file.c \
	   $(PATH_SRCS)/parse_file.c \
	   $(PATH_SRCS)/parsing.c

### DIRECTORIES & FILES
OBJ_DIR = .objs

#changer le path de la ligne ne dessous si je cree un dossier parsing et un dossier render
OBJS = $(patsubst $(PATH_SRCS)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_PARSING = $(patsubst $(PATH_SRCS)/%.c, $(OBJ_DIR)/%.o, $(SRCS_PARSING))

# **************************************************************************** #
#                                  COMPILATION                                 #
# **************************************************************************** #

### RULES OF COMPILATION
$(OBJ_DIR)/%.o: $(PATH_SRCS)/%.c $(HDRS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_MLX) -I $(PATH_LIBFT)/$(PATH_INCLUDE) -I $(PATH_SRCS) -I $(PATH_INCLUDE)

# **************************************************************************** #
#                                  MAIN TARGET                                 #
# **************************************************************************** #

### BUILD
all: $(LIBFT) $(MLX) $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LINKER_FLAGS) $(MLX)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I $(PATH_MLX) -I $(PATH_LIBFT) -I $(PATH_SRCS)

### LIBFT

$(LIBFT):
	$(MAKE) -C $(PATH_LIBFT)

### MLX
$(MLX):
	$(MAKE) -C $(PATH_MLX)

### CLEANING
clean:
	$(MAKE) -C $(PATH_LIBFT) clean
	$(MAKE) -C $(PATH_MLX) clean
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(PATH_LIBFT) fclean
	$(RM) $(NAME)
	$(RM) $(GTACTION)

### REBUILD
re: fclean all


# **************************************************************************** #
#                                  GITHUB ACTION                               #
# **************************************************************************** #

### NAME OF GITHUB ACTION
GTACTION = minirt_action

# Sources pour l'action (si différente)
SRCS_ACTION = $(PATH_SRCS)/check_compile.c
HDRS_ACTION = $(PATH_INCLUDE)/check_compile.h


# Objets pour l'action (si différente)
OBJS_DIR_ACTION = .objs_action
OBJS_ACTIONS = $(patsubst $(PATH_SRCS)/%.c, $(OBJS_DIR_ACTION)/%.o, $(SRCS_ACTION))


$(OBJS_DIR_ACTION)/%.o: $(PATH_SRCS)/%.c $(HDRS_ACTION)
	@mkdir -p $(OBJS_DIR_ACTION)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_MLX) -I $(PATH_LIBFT)/$(PATH_INCLUDE) -I $(PATH_SRCS) -I $(PATH_INCLUDE)

# Compilation de l'action (si différente)
$(GTACTION): $(OBJS_ACTIONS)
	$(CC) $(CFLAGS) -o $(GTACTION) $(OBJS_ACTIONS) $(LIBFT) $(LINKER_FLAGS) $(MLX)

# Construction de l'action (si différente)
action: $(LIBFT) $(MLX) $(GTACTION)

#remove $(MLX) From $(NAME) if parsing are call by make parsing
parsing: $(LIBFT) $(OBJS_PARSING)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_PARSING) $(LIBFT)

# **************************************************************************** #
#                                   PHONIES                                    #
# **************************************************************************** #

.PHONY: all clean fclean re action
