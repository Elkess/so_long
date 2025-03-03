NAME = so_long
SRCS = so_long.c helper_funcs.c parsing.c parsing_utils.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
# -fsanitize=address -g3
# LIBS = -Lmlx -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -lmlx -framework OpenGL -framework AppKit -o $(NAME)

%.o: %.c so_long.h
	@$(CC) $(CFLAGS) -Imlx -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

all: $(NAME)
