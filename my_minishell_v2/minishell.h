/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:34:43 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/18 14:21:55 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <string.h>

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define FIL 6
# define PIPE 7
# define END 8

# define MAX_VAR_LENGTH 256
# define MAX_VAR_NAME 256

# define TEMP_BUFFER_SIZE 1024

typedef struct s_quote_state {
    int dq;
    int sq;
} t_quote_state;

typedef enum e_quote_state
{
    NONE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_QuoteState;

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

typedef struct	s_heredoc
{
	struct s_heredoc	*next;
	char				*delimiter;
	int					fds[2];
}	t_heredoc;

typedef struct	s_section
{
	struct s_general	*info;
	struct s_section	*next;
	t_file				*files;
	t_heredoc			*heredocs;
	int					fd_read;
	int					fd_write;
	char				**env;
	char				**paths;
	char				*path;
	char				**cmdv;
	pid_t				pid;
}	t_section;

typedef	struct	s_general
{
	int					exit_status;
	int					number_of_tokens;
	struct s_token		*tokens_list;
	char				**env;
	char				**exports;
	char				**paths;
	struct s_section	*sections;
}	t_general;

typedef struct s_extract
{
    char            *current_token;
    int             i;
    int             is_first_token;
    int             expect_file;
    int             in_single_quotes;
    int             in_double_quotes;
    t_QuoteState    quote_state;
    int             *start_pos;
    int             size_malloc;
    int             j;
    int             z;
    int             length_difference;
    int             h;
}   t_extract;

typedef struct s_params
{
    char    *temp;
    int     temp_index;
    int     expanded;
}   t_params;

// tokens.c
t_token *tokenize_input(t_general *info, char *input);
t_token	*reverse_copy_list(t_token *tokens_list);

// sections.c
void        set_files_section(t_section *section, t_token *first, int s);
t_section   *create_sections_list(t_general *info);
void        set_cmdv_section(t_section *section, t_token *first, int s);

// executor.c
void    child_process(t_section *current, int prev_fd, int *pipefd);
void	executor(t_general *info);

// prints.c
void	put_str_fd(int fd, char *str);
void    print_tokens_list(t_token *tokens_list);
void    print_matrix(char **matrix);
void    print_string_to_stderror(char *str);
void    print_sections_info(t_section *section);

// frees.c
void	matrix_free(char **str);
void	free_sections_list(t_section *first);
void    free_tokens_list(t_token *first);
void	free_files(t_file *files);
void	free_info(t_general *info);

// utils_1.c
int		thereis_pipe(t_token *first);
void    set_paths_and_env(t_general *info, char **env);
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
int		exec_if_builtin_1(t_section *current);
void	exec_if_builtin_2(t_section *current);
char    *ft_strjoin_pipex(char const *s1, char const *s2);
void	set_cmd_in_paths(t_section *section);
void	set_path(t_section *section);

// utils_4.c
char    *add_var_equal(char *cmdv1);
char    **remove_env_line(t_section *current, int line);
int		is_directory(const char *path);
char	*ft_getenv(const char *name, char **env);
void	set_exports(t_general *info, char **env);

// utils_5.c
void	add_str_to_matrix(char ***matrix, char *str);
void    allocate_heredocs(t_section *section, t_token *first);
void	write_in_heredocs(t_section *current);
char    *clean_str_exit(char *str);
void    close_section_hdocs_parent(t_section *current);

// utils_6.c
void    init_section_objects_helper(t_section *section, t_token *first, int s);
void    set_cmdv_section_helper(t_section *section, char **cmdv, int i);
void    open_truncate(t_section *section, t_file *tmp);
void    open_append(t_section *section, t_file *tmp);
void    close_prev_fdread(t_section *section);

// utils_7.c
void    exit_if_no_cmdv(t_section *section, int *i);
void    set_cmd_in_paths_helper(t_section *section, char *new_path, int i);
int     count_lines(char **matrix);
void    allocate_heredocs_helper(t_token *tmp, t_heredoc *tmp_hdocs, t_heredoc *new_hdoc, int count);
void    clean_str_exit_helper(char *result, char *str);

// utils_8.c
void    write_in_heredocs_helper(t_heredoc *tmp_hdoc, char *buffer);
int		ft_atol_overflow(const char *str, long *result);
void	check_argc_exit(t_section *current);

// utils_9.c
char	*get_var_name(const char *var);
int		compare_var_name(const char *env_var, const char *var_name);
char	*get_prev_dir(char **env);
char	*get_home(char **env);

// utils_10.c
int		search_var_env(char ***env, char *var_name, size_t env_len);
int		check_all_env(char ***env, char *var, char *var_name, size_t *env_len);
void	set_new_env(char **new_env, size_t j, char *var_name, char ***env);
int		check_new_env(char **newenv, int i);
void	set_new_env_2(t_section *current, char **new_env, char *var_equal);

// utils_11.c
void	add_export_var(t_section *current, char **new_paths, int n);
void	change_pwd(t_section *current, char **content, char **var_pwd);
void	update_oldpwd(t_section *current, char **var_oldpwd, int *i);
char	*get_pdir_helper(size_t len, char **pwd, char **prev_dir);

// utils_executor.c
void    handle_child_process(t_section *current, int prev_fd, int pipefd[2]);
void    create_pipe_if_needed(t_section *current, int pipefd[2]);
void    wait_for_sections(t_general *info);

// builtins_1.c
int		execute_echo(t_section *current);
int		execute_pwd(t_section *current);
int		execute_env(t_section *current);

// builtins_2.c
void	if_remove_var_from_env(char ***env, char *var);
void	execute_unset(t_section *current);
void	execute_export(t_section *current);
void    execute_cd(t_section *current);
void	execute_exit(t_section *current);

// mele
const char	*get_token_type_name(int type);
int         check_syntax_errors(t_general *info);
void        free_tokens_list_tokenize(t_general *info);
int         open_quote(char *line, t_quote_state *state);
void        process_current_token(t_extract *ex, t_general *info);
void        handle_input_token(t_extract *ex, t_general *info);
void        handle_quotes(t_extract *ex, const char *section);
void        cleanup_extract(t_extract *ex);
void        aux(t_extract *ex, t_general *info);
void        process_section_2(t_extract *ex, t_general *info,
                const char *section);
void        initialize_extract(t_extract *ex);
void        add_token_and_cleanup(t_extract *ex, t_general *info,
                const char *token, int type);
void        handle_pipe_token(t_extract *ex, t_general *info,
                const char *section);
void        handle_space_token(t_extract *ex, t_general *info,
                const char *section);
void        handle_current_token_update(t_extract *ex, const char *section);
void        finalize_token(t_extract *ex, t_general *info);
void        handle_variable_expansion(t_extract *ex, t_general *info,
                const char *section);
void        process_current_token(t_extract *ex, t_general *info);
void        handle_input_redirect(t_extract *ex, t_general *info,
                const char *section);
void        handle_input_token(t_extract *ex, t_general *info);
void        reset_ex_values(t_extract *ex);
void        handle_trunc_redirect(t_extract *ex, t_general *info);
void        handle_append_redirect(t_extract *ex, t_general *info);
void        handle_single_quotes(t_extract *ex, const char *section);
void        handle_double_quotes(t_extract *ex, const char *section);
int         *allocate_and_initialize_positions(const char *section,
                int *size_malloc);
int         calculate_length_difference(const char *input,
                int start_pos, t_general *info);
void        expand_vars_for_length(const char *input, int start_pos,
                t_params *p, t_general *info);
int         handle_expansion_len(const char *input, int *i, t_params *p,
                t_general *info);
int         handle_dollar_question(t_params *p, int *i, t_general *info);
void        copy_initial_chars(const char *input, int start_pos, t_params *p);
char        *expand_variable(const char *input, int start_pos, t_general *info);
void        expand_variables_in_input(const char *input, int start_pos,
                t_params *p, t_general *info);
void        copy_until_start_pos(const char *input, int start_pos, t_params *p);
char        *handle_dollar_expansion(const char *input, int *i, t_params *p,
                t_general *info);
void        extract_var_name(const char *input, int *i, char *var_name,
                int *var_index);
char        *initialize_var_name(void);
int         reset_positions(int *start_pos, int size_malloc);
int         count_dollars(const char *section);
char        *expand_dollar_question(t_general *info);
void        init_extract_section(int *in_single_quotes, int *in_double_quotes);
char        *extract_section(char **start, const char *delimiters);
void        error_malloc(void);
void        error_strdup(void);
int         is_special_separator(char c);
char        *get_env_var(t_general *info, const char *var_name);
char        *add_char_to_token(char *token, char c);
void        add_token_to_list(t_general *info, const char *str, int type);
int         ft_isspace(char c);
int         init_general(t_general *info, char **env);
void        sigint_handler(int signo);
void        setup_signals(void);
void        init_history(const char *history_file);
void        cleanup(const char *history_file);
void        process_input(t_general *info, char *input, int *print_mode);
void        print_tokens_list_alb(t_general *info);
void        print_token_info(int i, t_token *current);
void        handle_input(t_general *info, char *input, int *print_mode);
void        handle_special_commands(const char *input, int *print_mode);
void        initialize_environment(t_general *info, char **env,
                const char *history_file);
void        initialize_program_settings(int argc, char **argv,
                const char **history_file, int *print_mode);

#endif
