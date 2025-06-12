.PHONY: all clean fclean re

# ***** VARIABLES ***** #

# --- Final executable name --- #

NAME = ft_ping

# --- Directories --- #

SRC_DIR = srcs/
OBJ_DIR = objs/
INC_DIR = includes/

# --- Files --- #

SRC_FILES = ft_ping.c \
			check_args.c \
			socket.c \
			errors.c \
			icmp_packet.c

INC_FILES = ft_ping.h

OBJ_FILES = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

DEPS = $(OBJ_FILES:.o=.d)

# --- Compilation --- #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
DFLAGS = -MMD -MP -I $(INC_DIR)

# --- Commands --- #

RM = rm -rf
MKDIR = mkdir -p

# ***** RULES ***** #

all:	$(NAME)

-include $(DEPS)

# --- Variables rules --- #

${NAME}:	$(OBJ_FILES)
			$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)

# --- Object files rules --- #

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(INC_DIR)$(INC_FILES)
				$(MKDIR) $(shell dirname $@)
				$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

# --- Usual rules --- #

clean:
			$(RM) $(OBJ_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean
			$(MAKE) all
