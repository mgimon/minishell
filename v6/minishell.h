/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:34:43 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/14 21:10:26 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

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

# define my_malloc(info, size) _my_malloc(info, size, __func__)
# define my_free(info, ptr) _my_free(info, ptr, __func__)

extern int				g_global_status;

typedef struct s_quote_state
{
	int					dq;
	int					sq;
}						t_quote_state;

typedef enum e_quote_state
{
	NONE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}						QuoteState;

typedef struct s_token
{
	char				*str;
	int					type;
	struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef struct s_file
{
	struct s_file		*next;
	char				*string;
	int					open_mode;
}						t_file;

typedef struct s_heredoc
{
	struct s_heredoc	*next;
	char				*delimiter;
	int					fds[2];
}						t_heredoc;

typedef struct s_section
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
}						t_section;

typedef struct
{
	void				*ptr;
	size_t				size;
	char				func_name[20];
}						AllocatedBlock;

typedef struct
{
	size_t				total_allocated;
	AllocatedBlock		allocated_blocks[16000];
	int					block_count;
	int					total_allocs;
	int					total_frees;
}						MemoryManager;

typedef struct s_general
{
	int					exit_status;
	int					number_of_tokens;
	struct s_token		*tokens_list;
	char				**env;
	char				**exports;
	char				**paths;
	struct s_section	*sections;
	MemoryManager		manager;
}						t_general;

// main.c
void					setup_signals(int i);

// tokens.c
t_token					*tokenize_input(t_general *info, char *input);
t_token					*reverse_copy_list(t_token *tokens_list);

// sections.c
void					set_files_section(t_section *section, t_token *first,
							int s);
t_section				*create_sections_list(t_general *info);
void					set_cmdv_section(t_section *section, t_token *first,
							int s);

// executor.c
void					child_process(t_section *current, int prev_fd,
							int *pipefd);
void					executor(t_general *info);

// prints.c
void					put_str_fd(int fd, char *str);
void					print_tokens_list(t_token *tokens_list);
void					print_matrix(char **matrix);
void					print_string_to_stderror(char *str);
void					print_sections_info(t_section *section);

// frees.c
void					matrix_free(char **str);
void					free_sections_list(t_section *first);
void					free_tokens_list(t_token *first);
void					free_files(t_file *files);
void					free_info(t_general *info);

// frees_2.c
void					free_heredocs(t_general *info);

// utils_1.c
int						thereis_pipe(t_token *first);
void					set_paths_and_env(t_general *info, char **env);
int						count_tokens_per_section(t_token *first);
int						count_sections(t_token *first);
int						ft_strncmp_pipex(const char *str, const char *str2,
							size_t c);

// utils_2.c
int						count_files_per_section(t_token *first);
int						count_cmdvs_per_section(t_token *first);
t_token					*get_first_in_section(t_token *first, int s);
void					add_file_to_files(t_token *section_first, t_file *files,
							int *i, int n);
void					open_files_section(t_section *section);

// utils_3.c
int						exec_if_builtin_1(t_section *current);
void					exec_if_builtin_2(t_section *current);
char					*ft_strjoin_pipex(char const *s1, char const *s2);
void					set_cmd_in_paths(t_section *section);
void					set_path(t_section *section);

// utils_4.c
char					*add_var_equal(char *cmdv1);
char					**remove_env_line(t_section *current, int line);
int						is_directory(const char *path);
char					*ft_getenv(const char *name, char **env);
void					set_exports(t_general *info, char **env);

// utils_5.c
void					add_str_to_matrix(char ***matrix, char *str);
void					allocate_heredocs(t_section *section, t_token *first,
							int s);
void					write_in_heredocs(t_section *current, t_general *info);
char					*clean_str_exit(char *str);
void					close_section_hdocs_parent(t_section *current);

// utils_6.c
void					init_section_objects_helper(t_section *section,
							t_token *first, int s);
void					set_cmdv_section_helper(t_section *section, char **cmdv,
							int i);
void					open_truncate(t_section *section, t_file *tmp);
void					open_append(t_section *section, t_file *tmp);
void					close_prev_fdread(t_section *section);

// utils_7.c
void					exit_if_no_cmdv(t_section *section, int *i);
void					set_cmd_in_paths_helper(t_section *section,
							char *new_path, int i);
int						count_lines(char **matrix);
void					allocate_heredocs_helper(t_token *tmp,
							t_heredoc *tmp_hdocs, t_heredoc *new_hdoc,
							int count);
void					clean_str_exit_helper(char *result, char *str);

// utils_8.c
void					write_in_heredocs_helper(t_heredoc *tmp_hdoc,
							char *buffer, t_general *info);
int						ft_atol_overflow(const char *str, long *result);
void					check_argc_exit(t_section *current);

// utils_9.c
char					*get_var_name(const char *var);
int						compare_var_name(const char *env_var,
							const char *var_name);
char					*get_prev_dir(char **env);
char					*get_home(char **env);
int						n_valid_succession(char **cmdv, int *i);

// utils_10.c
int						search_var_env(char ***env, char *var_name,
							size_t env_len);
int						check_all_env(char ***env, char *var, char **var_name,
							size_t *env_len);
void					set_new_env(char **new_env, size_t j, char *var_name,
							char ***env);
int						check_new_env(char **newenv, int i);
void					set_new_env_2(t_section *current, char **new_env,
							char *var_equal);

// utils_11.c
void					add_export_var(t_section *current, char ***new_paths,
							int n);
char					*get_pdir_helper(size_t len, char **pwd,
							char **prev_dir);
int						has_slash(char *str);
int						has_content(char *str);
int						n_all(char **cmdv);

// utils_12.c
void					remove_endslash(char **var);
int						remove_dots_helper(char *var_pwd, int *i, char c);
int						remove_dots(char **var_pwd);
void					remove_lastdir_helper(char *new_varpwd, char **var_pwd,
							int *i, int *n);
void					remove_var_from_exports(t_section *current,
							char *var_equal);

// utils_13.c
void					remove_lastdir(t_section *current, char **var_pwd);
void					change_last_subdir_helper(char **var_pwd, char *result,
							int *i, int *n);
void					change_last_subdir(t_section *current, char **var_pwd);
int						has_dots_and_subdir(char *str);
void					update_oldpwd(char **var_oldpwd, char **var_pwd);

// utils_14.c
void					upwds_one(t_section *current, char *tmp, char **var_pwd,
							char **var_oldpwd);
void					upwds_two(char **var_pwd, char **var_oldpwd);
void					upwds_three(t_section *current, char **var_pwd,
							char **var_oldpwd);
void					upwds_four(t_section *current, char *tmp,
							char **var_pwd, char **var_oldpwd);
void					upwds_five(t_section *current, char *tmp,
							char **var_pwd, char **var_oldpwd);

// utils_15.c
void					update_pwds(t_section *current, char **var_pwd,
							char **var_oldpwd);
char					*get_var_pwd(t_section *current);
char					*get_var_oldpwd(t_section *current);
int						count_exports(char **exports);
char					**copy_exports(char **exports, int omit_index,
							int total);

// utils_16
void					reset_newline_flag(char **cmdv, int *no_newline);
void					clean_repeateds(char ***env);
char					**reset_env(char ***env, int i);
int						count_env(char **env);
char					*give_varname(char *str);

// utils_17.c
void					if_backdirs_only(t_section *current);

// utils_18.c
void					remove_duplicates(char ***env, int i);
char					**copy_non_null_elements(char **env, int size);
void					free_and_nullify(char **ptr);

// utils_19.c
char					**allocate_path_parts(void);
char					*allocate_result(const char *path);
int						handle_double_dot(char **path_parts, int count);
int						copy_path_part(const char *start, int length,
							char **path_parts, int count);
void					free_allocated_parts(char **path_parts, int count,
							char *result);

// utils_20.c
int						process_path_segments(const char *path,
							char **path_parts);
void					build_clean_path(char *result, char **path_parts,
							int count);
void					copy_env_var(char **new_env, char **env, int i);
char					*extract_pwd(char *env_var);
char					*create_new_env_entry(char *new_pwd);

// utils_21.c
char					*clean_path(const char *path);
int						calculate_env_size(char **env);
void					free_and_replace_env(t_section *current,
							char **new_env);
void					process_env_var(char **new_env, char **env, int i);
void					if_clean_complex_route(t_section *current);

// utils_executor.c
void					handle_child_process(t_section *current, int prev_fd,
							int pipefd[2]);
void					create_pipe_if_needed(t_section *current,
							int pipefd[2]);
void					wait_for_sections(t_general *info);

// builtins_1.c
int						execute_echo(t_section *current);
int						execute_pwd(t_section *current);
int						execute_env(t_section *current);

// builtins_2.c
void					if_remove_var_from_env(char ***env, char *var);
void					execute_unset(t_section *current);
void					execute_export(t_section *current);
void					execute_cd(t_section *current);
void					execute_exit(t_section *current);

// tokenize_aux_1.c
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strcat(char *dest, const char *src);
size_t					get_old_size(char *ptr);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*trolft_strdup(const char *s1, t_general *info);

// tokenize_aux_2.c
const char				*get_token_type_name(int type);
void					free_tokens_list_tokenize(t_general *info);
void					quoting_choice(t_quote_state *state, char c);
int						open_quote(char *line, t_quote_state *state);

// tokenize_aux_3.c
char					*allocate_and_copy_section(char **start, char *end,
							t_general *info);
void					init_extract_section(int *in_single_quotes,
							int *in_double_quotes);
void					update_quotes(char current_char, int *in_single_quotes,
							int *in_double_quotes);
void					error_strdup(void);
void					error_malloc(void);

// tokenize_aux_4.c
char					*extract_section(char **start, const char *delimiters,
							t_general *info);
char					*append_string(char *dest, char *src);
void					add_token_to_list(t_general *info, const char *str,
							int type);
char					*add_char_to_token2(t_general *info, char *token,
							char c);
char					*add_char_to_token(t_general *info, char *token,
							char c);

// tokenize_aux_5.c
int						ft_isspace(char c);
char					*get_env_var(t_general *info, const char *var_name);
int						is_special_separator(char c);

// tokenize_aux_6.c
int						count_dollars(const char *section);
void					print_start_pos(int *start_pos);
int						reset_positions(int *start_pos, int size_malloc);

// tokenize_aux_7.c
char					*expand_dollar_question(t_general *info);
char					mark_char(char c);
char					unmark_char(char c);
int						is_marked(char c);

// tokenize_aux_8.c

// trol_alloc.c
void					*_my_malloc(t_general *info, size_t size,
							const char *func_name);
void					_my_free(t_general *info, void *ptr,
							const char *func_name);
void					free_all_allocated_blocks(t_general *info);
char					*my_realloc_char(char *ptr, size_t new_size);

const char				*get_token_type_name(int type);
int						check_syntax_errors(t_general *info);
void					free_tokens_list_tokenize(t_general *info);
int						open_quote(char *line, t_quote_state *state);
char					*extract_current_section(const char *section,
							t_general *info);
char					mark_char(char c);
char					unmark_char(char c);
int						is_marked(char c);
char					*expand_variable(const char *input, int start_pos,
							t_general *info);
char					*expand_dollar_question(t_general *info);
int						is_special_separator(char c);
char					*get_env_var(t_general *info, const char *var_name);

char					*trolft_strdup(const char *s1, t_general *info);
char					*trolft_itoa(int n, t_general *info);
int						check_syntax_errors(t_general *info);
#endif
