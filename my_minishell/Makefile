NAME        = minishell
HEADER      = minishell.h
FLAGS       = -Wall -Werror -Wextra
LIB_DIR     = ./libft/
LIB_NAME    = libft.a
LIB         = $(addprefix $(LIB_DIR), $(LIB_NAME))

SRCS        = main.c tokenizer.c tokenizer_2.c executor.c prints.c \
				utils_1.c sections.c utils_2.c \
				utils_3.c builtins_1.c frees.c \
				builtins_2.c utils_4.c \

OBJS        = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) -L$(LIB_DIR) -lft -lreadline -fsanitize=address

$(LIB):
	@make -C $(LIB_DIR)

%.o : %.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@make clean -C $(LIB_DIR)
	rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIB_DIR)
	rm -f $(NAME)

re: fclean all
