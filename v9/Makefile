NAME        = minishell
HEADER      = minishell.h
FLAGS       = -Wall -Werror -Wextra -fsanitize=address -fsanitize=leak
LIB_DIR     = ./libft/
LIB_NAME    = libft.a
LIB         = $(addprefix $(LIB_DIR), $(LIB_NAME))

SRCS        = main.c executor.c prints.c \
				utils_1.c sections.c utils_2.c \
				utils_3.c builtins_1.c frees.c frees_2.c \
				builtins_2.c utils_4.c token_1.c \
				utils_5.c utils_executor.c \
				utils_6.c utils_7.c utils_8.c \
				utils_9.c utils_10.c utils_11.c \
				utils_12.c utils_13.c utils_14.c \
				utils_15.c utils_16.c utils_17.c \
				utils_18.c utils_19.c utils_20.c \
				utils_21.c utils_22.c \
				trolitoa.c tokenize_aux_1.c  trol_alloc.c\
				tokenize_aux_2.c tokenize_aux_3.c tokenize_aux_4.c\
				tokenize_aux_5.c tokenize_aux_6.c tokenize_aux_7.c\
				tokenize_aux_8.c tokenize_aux_9.c aux_expansor.c\
				aux_aux_expansor.c syntax.c aux_sintax.c extract_token.c\
				extract_token_aux.c extract_token_aux_aux.c extract_section.c\
				extract_section_aux.c extract_section_aux_aux.c extract_section_aux_aux_aux.c\
				extract_section_aux_aux_aux_aux.c \
				utils_23.c utils_24.c utils_25.c utils_26.c utils_27.c \


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
