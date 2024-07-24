/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:34:43 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/24 20:14:51 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define FIL 6
# define PIPE 7
# define END 8

typedef struct	s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct	s_file
{
	t_file	*next;
	char	*string;
	int		open_mode;
}

typedef struct	s_section
{
	struct s_section	*next;
	t_file				*files;
	int					fd_read;
	int					fd_write;
	char				**env;
	char				*path;
	char				**cmdv;
}	t_section

typedef	struct	s_general
{
	int				number_of_tokens;
	struct s_token	*tokens_list;
}	t_general;

// tokenizer.c
void	tokenizer(t_general *info, char *input);

// sections.c
char    **get_gross_section(t_token *first, int *section_end);

// executor.c
void	tokens_executor(t_general *info);

// prints.c
void    print_token_list(t_general *info);
void    print_matrix(char **matrix);

#endif
