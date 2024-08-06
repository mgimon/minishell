/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:34:43 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/06 18:15:54 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
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
	struct s_file	*next;
	char			*string;
	int				open_mode;
}	t_file;

typedef struct	s_section
{
	struct s_section	*next;
	t_file				*files;
	int					fd_read;
	int					fd_write;
	char				**env;
	char				**paths;
	char				*path;
	char				**cmdv;
}	t_section;

typedef	struct	s_general
{
	int					number_of_tokens;
	struct s_token		*tokens_list;
	char				**env;
	char				**paths;
	struct s_section	*sections;
}	t_general;

// tokenizer.c
void	tokenizer(t_general *info, char *input);

// sections.c
t_section	*create_sections_list(t_general *info);

// executor.c
void	executor(t_general *info);

// prints.c
void    print_token_list(t_general *info);
void    print_matrix(char **matrix);
void    print_sections_info(t_section *section);

// utils_1.c
int		thereis_pipe(t_token *first);
void    set_path_and_env(t_general *info, char **env);
int		count_tokens_per_section(t_token *first);
int		count_sections(t_token *first);
int		ft_strncmp_pipex(const char *str, const char *str2, size_t c);

// utils_2.c
int		count_files_per_section(t_token *first);
int		count_cmdvs_per_section(t_token *first);
t_token *get_first_in_section(t_token *first, int s);
void    add_file_to_files(t_token *section_first, t_file *files, int *i, int n);
void    open_files_section(t_section *section);

// utils_3.c
int		exec_if_builtin(t_section *current);
char    *ft_strjoin_pipex(char const *s1, char const *s2);
void	set_cmd_in_paths(t_section *section);
void	set_path(t_section *section);

// tokenizer_2.c
int		recalculate_tokens(t_general *info);
void	add_token(t_general *info, char *word, int type);
void	tokenize_input(t_general *info, char *input);
void	free_tokens_list(t_general *info);

// builtins.c
int		execute_echo(t_section *current);

#endif
