NAME        = minishell
HEADER      = minishell.h
FLAGS       = -Wall -Werror -Wextra
LIB_DIR     = ./libft/
LIB_NAME    = libft.a
LIB         = $(addprefix $(LIB_DIR), $(LIB_NAME))

SRCS        = main.c tokenizer.c executor.c
OBJS        = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) -L$(LIB_DIR) -lft -lreadline

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
