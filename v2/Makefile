NAME        = minishell
HEADER      = minishell.h
FLAGS       = -Wall -Werror -Wextra -fsanitize=address -fsanitize=leak
LIB_DIR     = ./libft/
LIB_NAME    = libft.a
LIB         = $(addprefix $(LIB_DIR), $(LIB_NAME))

SRCS        = main.c executor.c prints.c \
				utils_1.c sections.c utils_2.c \
				utils_3.c builtins_1.c frees.c \
				builtins_2.c utils_4.c token_1.c \
				utils_5.c utils_executor.c \
				utils_6.c utils_7.c utils_8.c \
				utils_9.c utils_10.c utils_11.c \
				utils_12.c utils_13.c utils_14.c \
				utils_15.c utils_16.c utils_17.c \
				#token_2.c token_3.c \
				token_4.c token_5.c token_6.c token_7.c \
				token_8.c token_9.c token_10.c token_11.c \
				aux.c aux2.c \

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
