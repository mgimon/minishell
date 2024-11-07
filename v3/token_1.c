/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:10:59 by albealva          #+#    #+#             */
/*   Updated: 2024/11/07 19:12:30 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void extract_tokens(const char *section, t_general *info);

char *ft_strcpy(char *dest, const char *src)
{
    char *dest_copy;

    dest_copy = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return dest_copy;
}

char *ft_strcat(char *dest, const char *src)
{
    size_t i;
    size_t j;

    i = 0;
    j = 0;
    while (dest[i] != '\0')
        i++;
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return (dest);
}

size_t get_old_size(char *ptr)
{
    size_t size = 0;

    if (ptr == NULL)
        return 0; 
    while (ptr[size] != '\0') 
        size++;
    return size;
}

char* my_realloc_char(char* ptr, size_t new_size)
{
    size_t old_size;
    size_t i;
    char* new_ptr;
    size_t size_to_copy;
    
    old_size = get_old_size(ptr);
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return (char*)malloc(new_size);
    new_ptr = (char*)malloc(new_size);
    if (new_ptr == NULL)     
        return NULL;
    i = -1;
    if (old_size < new_size)
        size_to_copy = old_size;
    else
        size_to_copy = new_size;
    while (i++ < size_to_copy)
        new_ptr[i] = ptr[i];
    free(ptr);
    return new_ptr;
}

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }

    return (dest);
}
void* _my_malloc(t_general *info, size_t size, const char* func_name)
{
    MemoryManager *manager;
    
    manager = &info->manager;
    if (manager == NULL) 
        return NULL;

    void* ptr = malloc(size);
    if (ptr != NULL) {
        manager->total_allocated += size;
        manager->allocated_blocks[manager->block_count].ptr = ptr;
        manager->allocated_blocks[manager->block_count].size = size;
        ft_strncpy(manager->allocated_blocks[manager->block_count].func_name, func_name, sizeof(manager->allocated_blocks[manager->block_count].func_name) - 1);
        manager->allocated_blocks[manager->block_count].func_name[sizeof(manager->allocated_blocks[manager->block_count].func_name) - 1] = '\0';
        manager->block_count++;
        manager->total_allocs++;
    }
    return ptr;
}

void _my_free(t_general *info, void* ptr, const char* func_name)
{
    MemoryManager *manager;
    
    (void)func_name;
    manager = &info->manager;
    if (manager == NULL)
        return;

    for (int i = 0; i < manager->block_count; i++) {
        if (manager->allocated_blocks[i].ptr == ptr) {
            manager->total_allocated -= manager->allocated_blocks[i].size;
            free(ptr);
            manager->allocated_blocks[i] = manager->allocated_blocks[--manager->block_count];
            return;
        }
    }
    //printf("Attempted to free unallocated memory at %p in function %s\n", ptr, func_name);
}

void free_all_allocated_blocks(t_general *info)
{
    MemoryManager *manager;

    manager = &info->manager;
    if (manager == NULL) 
        return;

    for (int i = 0; i < manager->block_count; i++) {
        free(manager->allocated_blocks[i].ptr);
    }
    manager->block_count = 0;
    manager->total_allocated = 0;
    //free(manager);
    //manager = NULL;
}

char	*trolft_strdup(const char *s1, t_general *info)
{
	char	*s1_copy;
	size_t	i;

	if (s1 == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		i++;
	}
	s1_copy = my_malloc(info, i + 1);
	if (s1_copy == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s1_copy[i] = (char)s1[i];
		i++;
	}
	s1_copy[i] = '\0';
	return ((char *)s1_copy);
}

const char	*get_token_type_name(int type)
{
	static const char *token_type_names[9]; 

	token_type_names[0] = "EMPTY";
	token_type_names[1] = "CMD";
	token_type_names[2] = "ARG";
	token_type_names[3] = "TRUNC";
	token_type_names[4] = "APPEND";
	token_type_names[5] = "INPUT";
	token_type_names[6] = "FIL";
	token_type_names[7] = "PIPE";
	token_type_names[8] = "END";
	if (type >= 0 && type <= 8)
		return token_type_names[type];
	else
		return "UNKNOWN";
	
}


void	free_tokens_list_tokenize(t_general *info)
{
	t_token *current;
	t_token *next;

	current = info->tokens_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	info->tokens_list = NULL;
	info->number_of_tokens = 0;
}

void	quoting_choice(t_quote_state *state, char c)
{
	if (c == '"' && state->sq == 0)
	{
		if (state->dq == 0)
			state->dq = 1;
		else
			state->dq = 0;
	} 
	else if (c == '\'' && state->dq == 0)
	{
		if (state->sq == 0)
			state->sq = 1;
		else
			state->sq = 0;
	}
}

int	open_quote(char *line, t_quote_state *state)
{
	int i;

	i = 0;
	state->dq = 0;
	state->sq = 0;
	while (line[i] != '\0')
	{
		quoting_choice(state, line[i]);
		i++;
	}
	if (state->dq != 0 || state->sq != 0)
	{
		printf("unclosed quotes error \n");
		return 1;
	}
	return 0;
}
void	error_malloc()
{
	perror("Malloc failed");
	exit(EXIT_FAILURE);
}

void	error_strdup()
{
	perror("Strdup failed");
	exit(EXIT_FAILURE);
}

void update_quotes(char current_char, int *in_single_quotes, int *in_double_quotes)
{
    if (current_char == '\'')
        *in_single_quotes = !(*in_single_quotes);
    else if (current_char == '\"')
        *in_double_quotes = !(*in_double_quotes);
}

void init_extract_section(int *in_single_quotes, int *in_double_quotes)
{
	*in_single_quotes = 0;
	*in_double_quotes = 0;
}

char *allocate_and_copy_section(char **start, char *end, t_general *info)
{
    size_t section_length = end - *start;
    char *section = my_malloc(info, section_length + 1);
    if (!section) {
        error_malloc();
    }
    ft_strncpy(section, *start, section_length);
    section[section_length] = '\0';
    *start = end;
    return section;
}


char	*extract_section(char **start, const char *delimiters, t_general *info)
{
	char *end;
	char *section;
	int in_single_quotes;
	int in_double_quotes;
	
	init_extract_section(&in_single_quotes, &in_double_quotes);
	if (**start == '|') {
		section = my_malloc(info, 2);
		if (!section)
			error_malloc();
		section[0] = '|';
		section[1] = '\0';
		(*start)++;
		return section;
	}
	end = *start;
	while (*end && (!strchr(delimiters, *end) || in_single_quotes || in_double_quotes))
	{
		update_quotes(*end, &in_single_quotes, &in_double_quotes);
		end++;
	}
	section = allocate_and_copy_section(start, end, info);
	return section;
}

char *append_string(char *dest, char *src)
{
    size_t src_len ;
    size_t dest_len ;
    
    if (dest == NULL || src == NULL)
        return (NULL);
    dest_len = ft_strlen(dest);
    src_len = ft_strlen(src);
    dest = my_realloc_char(dest, dest_len + src_len + 1);
    if (dest == NULL)
    {
        fprintf(stderr, "Error reallocating memory\n");
        return NULL;
    }
    ft_strcat(dest, src);
    return dest;
}


void add_token_to_list(t_general *info, const char *str, int type)
{
    t_token *new_token;
    t_token *last;
    
    new_token = my_malloc(info, sizeof(t_token));
    if (!new_token)
        error_malloc();
    new_token->str = trolft_strdup(str, info);
    if (!new_token->str)
        error_malloc();
    new_token->type = type;
    new_token->prev = NULL;
    new_token->next = NULL;
    if (!info->tokens_list)
        info->tokens_list = new_token;
    else
    {
        last = info->tokens_list;
        while (last->next)
            last = last->next;
        last->next = new_token;
        new_token->prev = last;
    }
    info->number_of_tokens++;
}

char *add_char_to_token2(t_general *info ,char *token, char c)
{
    int     len;
    char    *new_token;
    
    if (token != NULL)
        len = strlen(token);
    else
        len = 0; 
    new_token = my_malloc(info, len + 2);
    if (!new_token) 
        return NULL;
    if (token)
        ft_strcpy(new_token, token);
    new_token[len] = c;
    new_token[len + 1] = '\0';
    return new_token;
}

char *add_char_to_token(t_general *info, char *token, char c)
{
    
    int     len;
    char    *new_token;
    
    if (token != NULL)
        len = strlen(token);
    else
        len = 0; 
    new_token = my_malloc(info, len + 2); 
    if (!new_token) 
        return NULL;
    if (token) 
        ft_strcpy(new_token, token);
    if (is_marked(c))
        c=unmark_char(c);
    new_token[len] = c;
    new_token[len + 1] = '\0';
    return new_token;
}

int ft_isspace(char c) {
    return (c == ' ' || c == '\t' || c== '\r');
}


bool should_expand(bool in_single_quotes)
{
    return !in_single_quotes;
}

typedef enum
{
    NONE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} QuoteState;


char *get_env_var(t_general *info, const char *var_name)
{
    int i;
    char *equal_sign;
    size_t name_length;

    i = 0;
    while (info->env[i] != NULL)
    {
        equal_sign = strchr(info->env[i], '=');
        if (equal_sign != NULL)
        {
            name_length = equal_sign - info->env[i];
            if (ft_strncmp(info->env[i], var_name, name_length) == 0 &&
                name_length == strlen(var_name))
                    return equal_sign + 1;
        }
        i++;
    }
    return ""; 
}

int is_special_separator(char c)
{
    return c == ' ' || c == '\t' || c == '\'' || c == '\"' || c == '<' || c == '>' || c == '|' || c == '$';
}





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

// Variables globales
int last_exit_status = 0; // Variable global para almacenar el último código de salida


void set_last_exit_status(int status)
{
    last_exit_status = status;
}

char* expand_dollar_question(t_general *info)
{
    char    *result;
    
    result = (char*)my_malloc(info, 12);
    if (result == NULL)
        return NULL;
    result = trolft_itoa(info->exit_status, info);
    return result;
}

void execute_command(const char* command)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp(command, command, NULL);
        exit(127);
    } else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            set_last_exit_status(WEXITSTATUS(status));
        }
        else

            set_last_exit_status(1);
    } 
    else
    {
        perror("fork failed");
        exit(1);
    }
}

int count_dollars(const char *section)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (section[i] != '\0')
	{
    if (section[i] == '$')
        count++;
    i++;
	}
	return (count);
}

void print_start_pos(int *start_pos)
{
	int i;
	
	i = 0;
	printf("Contenido de start_pos: ");
	while (start_pos[i])
	{
        printf("%d ", start_pos[i]);
		i++;
	}
	printf("\n");
}

int reset_positions(int *start_pos, int size_malloc)
{
	int k;
	
	k = 0;
	if(start_pos == NULL)
		return (-1);
	while (k < size_malloc)
	{
    	start_pos[k] = -1;
    	k++;
	}
	return (0);
}

char	mark_char(char c)
{
	return (c | 0x80);
}

char	unmark_char(char c)
{
	return (c & 0x7F);
}

int	is_marked(char c)
{
    return (c & 0x80) != 0;
}


char *expand_variable2(const char *input, int start_pos, t_general *info)
{
    char *result = NULL;
    int len = strlen(input);
    char temp[1024] = {0};  // Buffer temporal para construir el resultado expandido
    char var_name[256] = {0};
    int temp_index = 0;  // Índice para el buffer temporal
    int var_index = 0;
    int expanded = 0;  // Bandera para saber si ya hemos expandido una variable
    char* exit_status_str;
    // Copia los caracteres iniciales antes de start_pos
    for (int j = 0; j < start_pos - 1; j++)
    {
        temp[temp_index++] = input[j];
    }
    if (start_pos == 0) {
        temp[temp_index++] = input[0];
    }

    for (int i = start_pos - 1; i < len; i++) {
        // Expandir solo a partir de start_pos y solo una vez
        if (input[i] == '$' && input[i + 1] == '?' && !expanded) {
            i += 1;  // Avanza para saltarse el '?'
            // Expande "$?"
            exit_status_str = expand_dollar_question(info);
            if (exit_status_str != NULL) {
                strcpy(temp + temp_index, exit_status_str);
                temp_index += strlen(exit_status_str);
                //my_free(exit_status_str);
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
            continue;
        }
        
        if (input[i] == '$' && !expanded) {
            i++;  // Avanza para saltarse el '$'
            var_index = 0;
            // Captura el nombre de la variable de entorno
            while (i < len && !is_special_separator(input[i]) && (isalnum(input[i]) || input[i] == '_')) {
                var_name[var_index++] = input[i++];
            }
            i--;  // Retrocede un carácter porque el bucle ha avanzado uno de más.
            var_name[var_index] = '\0';

            // Obtiene el valor de la variable de entorno
            char *value = get_env_var(info, var_name);
            if (value) {
                // Marca comillas dentro del valor de la variable
                for (int k = 0; value[k] != '\0'; k++) {
                    if (value[k] == '"' || value[k] == '\'' || value[k] == '$') {
                        temp[temp_index++] = mark_char(value[k]);  // Marca comillas
                    } else {
                        temp[temp_index++] = value[k];  // Copia el carácter normalmente
                    }
                }
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
        } else {
            // Copia el contenido literal al buffer temporal
            temp[temp_index++] = input[i];
        }
    }

    // Duplica el resultado expandido
    //free(exit_status_str);
    result = trolft_strdup(temp,info);
    return result;
}


char *expand_variable(const char *input, int start_pos,t_general *info) {
   // printf("expand_variable called with input: %s, start_pos: %d\n", input, start_pos);
   // fflush(stdout);

    char *result = NULL;
    int len = strlen(input);
    char temp[1024] = {0};  // Buffer temporal para construir el resultado expandido
    char var_name[256] = {0};
    int temp_index = 0;  // Índice para el buffer temporal
    int var_index = 0;
    int expanded = 0;  // Bandera para saber si ya hemos expandido una variable
    char* exit_status_str;
    
    // Copia los caracteres iniciales antes de start_pos
    for (int j = 0; j < start_pos - 1; j++) {
        temp[temp_index++] = input[j];
    }
    if (start_pos == 0) {
        temp[temp_index++] = input[0];
    }
    for (int i = start_pos - 1; i < len; i++) {
        // Expandir solo a partir de start_pos y solo una vez
        if (input[i] == '$' && input[i + 1] == '?' && !expanded) {
            i += 1;  // Avanza para saltarse el '?'
            // Expande "$?"
            exit_status_str = expand_dollar_question(info);
            if (exit_status_str != NULL) {
                strcpy(temp + temp_index, exit_status_str);
                temp_index += strlen(exit_status_str);
                //my_free(exit_status_str);
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
            //result = strdup(temp);
            //return result;
            i++;  // Avanza para saltarse el '?'
        }
        if (input[i] == '$' && !expanded) {
            i++;  // Avanza para saltarse el '$'
            var_index = 0;
            // Captura el nombre de la variable de entorno
            while (i < len && !is_special_separator(input[i]) && (isalnum(input[i]) || input[i] == '_')) {
                var_name[var_index++] = input[i++];
            }
            i--;  // Retrocede un carácter porque el bucle ha avanzado uno de más.
            var_name[var_index] = '\0';

            // Obtiene el valor de la variable de entorno
            char *value = get_env_var(info,var_name);
            if (value) {
                strcpy(temp + temp_index, value);
                temp_index += strlen(value);
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
        } else {
            // Copia el contenido literal al buffer temporal
            //if(input[i] == '?') {
                //i += 1;  // Avanza para saltarse el '?'
            //}
            temp[temp_index++] = input[i];
        }
    }

    // Duplica el resultado expandido
    result = trolft_strdup(temp, info);
   // printf("expand_variable returning: %s\n", result);
   // fflush(stdout);
    //free(exit_status_str);
    return result;
}

int calculate_length_difference(const char *input, int start_pos,t_general *info) {
   // printf("calculate_length_difference called with input: %s, start_pos: %d\n", input, start_pos);
   // fflush(stdout);

    int original_len = strlen(input);
    char temp[1024] = {0};  // Buffer temporal para construir el resultado expandido
    char var_name[256] = {0};
    int temp_index = 0;  // Índice para el buffer temporal
    int var_index = 0;
    int expanded = 0;  // Bandera para saber si ya hemos expandido una variable
    char* exit_status_str;
    // Copia los caracteres iniciales antes de start_pos
    for (int j = 0; j < start_pos - 1; j++) {
        temp[temp_index++] = input[j];
    }
    if (start_pos == 0) {
        temp[temp_index++] = input[0];
    }
    for (int i = start_pos - 1; i < original_len; i++) {
        // Expandir solo a partir de start_pos y solo una vez
        if (input[i] == '$' && input[i + 1] == '?' && !expanded) {
            i += 1;  // Avanza para saltarse el '?'
            // Expande "$?"
            exit_status_str = expand_dollar_question(info);
            if (exit_status_str != NULL) {
                strcpy(temp + temp_index, exit_status_str);
                temp_index += strlen(exit_status_str);
                //my_free(exit_status_str);
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
            i++;  // Avanza para saltarse el '?'
        }
        if (input[i] == '$' && !expanded) {
            i++;  // Avanza para saltarse el '$'
            var_index = 0;
            // Captura el nombre de la variable de entorno
            while (i < original_len && !is_special_separator(input[i]) && (isalnum(input[i]) || input[i] == '_')) {
                var_name[var_index++] = input[i++];
            }
            i--;  // Retrocede un carácter porque el bucle ha avanzado uno de más.
            var_name[var_index] = '\0';

            // Obtiene el valor de la variable de entorno
            char *value = get_env_var(info,var_name);
            if (value) {
                strcpy(temp + temp_index, value);
                temp_index += strlen(value);
            }
            expanded = 1;  // Marca que ya hemos expandido una variable
        } else {
            // Copia el contenido literal al buffer temporal
            //if(input[i] == '?') {
                //i += 1;  // Avanza para saltarse el '?'
            //}
            temp[temp_index++] = input[i];
        }
    }

    int expanded_len = strlen(temp);  // Longitud del texto expandido
    int length_difference = expanded_len - original_len;

   // printf("calculate_length_difference returning: %d\n", length_difference);
   // fflush(stdout);
    //free(exit_status_str);
    return length_difference;
}








// Función para buscar un comando en los directorios de PATH
int is_valid_command(t_general *info, const char *command, char **env) {
    (void)env;
    char *path = getenv("PATH");  // Obtener el PATH del entorno
    if (!path) {
        fprintf(stderr, "Error: PATH variable not found.\n");
        return 0;
    }

    char *path_copy = trolft_strdup(path, info);  // Hacemos una copia de PATH para no modificarlo
    char *dir = strtok(path_copy, ":");  // Dividimos PATH por ":"

    // Creamos un buffer para las rutas completas de los comandos
    char full_path[512];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);  // Construimos la ruta completa

        if (access(full_path, X_OK) == 0) {  // Verificamos si el comando es ejecutable
            //my_free(path_copy);
            return 1;  // El comando existe y es ejecutable
        }

        dir = strtok(NULL, ":");  // Siguiente directorio en PATH
    }

    //my_free(path_copy);
    return 0;  // El comando no se encontró
}

// Función para verificar el primer token de la lista

int check_syntax_errors(t_general *info) {
    t_token *current_token = info->tokens_list;
    int last_type = 0;
    int error_found = 0;

    // Verificar el primer token
    if (current_token && current_token->type == CMD) {
        if (!is_valid_command(info, current_token->str, info->env)) {
            fprintf(stderr, "Error: Command '%s' not found.\n", current_token->str);
            return 1;  // Error de sintaxis
        }
        last_type = CMD; // Actualiza el tipo del último token
    } else {
        fprintf(stderr, "Error: No command found.\n");
        return 1;  // Error de sintaxis
    }

    // Avanzar al siguiente token
    current_token = current_token->next;

    while (current_token != NULL) {
        // Verifica el tipo de token actual
        if (current_token->type == CMD) {
            if (!is_valid_command(info, current_token->str, info->env)) {
                fprintf(stderr, "Error: Command '%s' not found.\n", current_token->str);
                return 1;  // Error de sintaxis
            }
            last_type = CMD; // Actualiza el tipo del último token
        } else if (current_token->type == ARG) {
            // Argumento puede seguir a un comando, otro argumento, o un operador de redirección
            if (last_type == PIPE) {
                fprintf(stderr, "Error: Argument '%s' in invalid position after pipe.\n", current_token->str);
                error_found = 1;  // Error de sintaxis
                break;
            }
            last_type = ARG; // Actualiza el tipo del último token
        } else if (current_token->type == TRUNC || current_token->type == APPEND) {
            // Redirección debe seguir a un comando o argumento
            if (last_type != CMD && last_type != ARG) {
                fprintf(stderr, "Error: Redirection operator '%d' in invalid position.\n", current_token->type);
                error_found = 1;  // Error de sintaxis
                break;
            }
            // Verifica que el siguiente token sea un archivo
            if (current_token->next == NULL || current_token->next->type != FIL) {
                fprintf(stderr, "Error: No file specified for redirection '%d'.\n", current_token->type);
                error_found = 1;  // Error de sintaxis
                break;
            }
            // Actualiza last_type después de procesar el archivo
            last_type = FIL;
            current_token = current_token->next; // Avanza al siguiente token
        } else if (current_token->type == INPUT) {
            // Redirección de entrada debe seguir a un comando o argumento
            if (last_type != CMD && last_type != ARG) {
                fprintf(stderr, "Error: Input redirection operator '%d' in invalid position.\n", current_token->type);
                error_found = 1;  // Error de sintaxis
                break;
            }
            // Verifica que el siguiente token sea un archivo
            if (current_token->next == NULL || current_token->next->type != FIL) {
                fprintf(stderr, "Error: No file specified for input redirection.\n");
                error_found = 1;  // Error de sintaxis
                break;
            }
            // Actualiza last_type después de procesar el archivo
            last_type = FIL;
            current_token = current_token->next; // Avanza al siguiente token
        } else if (current_token->type == PIPE) {
            // La tubería debe seguir a un comando o argumento
            if (last_type == PIPE || last_type == TRUNC || last_type == APPEND || last_type == INPUT) {
                fprintf(stderr, "Error: Pipe operator in invalid position.\n");
                error_found = 1;  // Error de sintaxis
                break;
            }
            last_type = PIPE; // Actualiza el tipo del último token
        } else {
            fprintf(stderr, "Error: Unknown token type '%d'.\n", current_token->type);
            error_found = 1;  // Error de sintaxis
            break;
        }

        // Avanza al siguiente token
        current_token = current_token->next;
    }

    // Verifica que el último token no sea un operador sin un archivo o comando siguiente
    if (!error_found && (last_type == TRUNC || last_type == APPEND || last_type == INPUT || last_type == PIPE)) {
        fprintf(stderr, "Error: Command ends with an invalid operator.\n");
        error_found = 1;  // Error de sintaxis
    }

    return error_found ? 1 : 0;  // Retorna 1 si hubo un error, 0 si no
}

void process_section(char *section, t_general *info)
{
    if (section)
    {
        extract_tokens(section, info);
        section = NULL;
    }
}

t_token *tokenize_input(t_general *info, char *input)
{
    t_quote_state state;
    open_quote(input, &state);
    char *section;
    char *start = input;
    const char *section_delimiters = "|\n\r";

    info->tokens_list = NULL;
    //printf("start: %s\n", start);
    while (*start) {
        section = extract_section(&start, section_delimiters, info);
        if (section) {
            process_section(section, info);
        }
    }
    //free(section);
    return info->tokens_list;
}
char *my_sprintf_concat(const char *str1, const char *str2, t_general *info)
{
    int len1;
    int len2;
    
    len1 = 0;
    len2 = 0;

    while (str1[len1] != '\0')
        len1++;    
    while (str2[len2] != '\0')
        len2++;
    char *result = (char *)my_malloc(info, len1 + len2 + 2);
    if (!result)
        error_malloc();
    ft_strcpy(result, str1);
    ft_strcat(result, " ");
    ft_strcat(result, str2);

    return result;
}

char *append_to_current_section(t_general *info, char *current_section, const char *token)
{
    char *temp;
    
    if (current_section == NULL)
    
        current_section = trolft_strdup(token, info);
    else
    {
        temp = my_malloc(info, strlen(current_section) + strlen(token) + 2);
        if (temp == NULL)
            error_malloc();
        temp = my_sprintf_concat(current_section, token, info);
        //sprintf(temp, "%s %s", current_section, token); // Concatenar con un espacio
        //my_free(current_section); // Liberar la sección anterior
        current_section = temp; // Asignar la nueva sección
    }
    return current_section; // Devolver la nueva cadena concatenada
}

static void trim_trailing_whitespace(char *str)
{
    int len = strlen(str);

    // Encuentra la posición final sin espacios ni tabulaciones
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }

    // Coloca '\0' desde la nueva longitud hasta el final original
    while (str[len] != '\0') {
        str[len] = '\0';
        len++;
    }
}

char *extract_current_section(const char *section, t_general *info) {
    char *current_token = NULL;  // Única variable para construcción de tokens
    int i = 0;
    //int is_first_token = 1;      // Indicador de primer token en la sección
    //int expect_file = 0;         // Indicador para identificar si el próximo token es un FILE
    int in_single_quotes = 0;    // Controla el estado de las comillas simples
    int in_double_quotes = 0;    // Controla el estado de las comillas dobles
    QuoteState quote_state = NONE; // Inicialización de la variable de estado
    int *start_pos = NULL;
    int size_malloc;
    int j = 0;
    int z = 0;
    int length_difference = 0;
    int h=0;
    char *current_section = NULL;
    //size_t pos_ini_8 = 8;  // Posición inicial fija para pruebas

    if (section && count_dollars(section) > 0) {
        size_malloc = count_dollars(section);
        start_pos = my_malloc(info, size_malloc * sizeof(int)); // size_t
        if (start_pos == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        for (int k = 0; k < size_malloc; k++) {
            start_pos[k] = -1;
        }
        //print_start_pos(start_pos);
    }

    
    while (section[i] != '\0') {

         // Manejo del salto de línea ('\n')
    if (section[i] == '\n' && !in_single_quotes && !in_double_quotes) {
        if (current_token) {
            if (quote_state != SINGLE_QUOTE && count_dollars(section)) {
                while (z < size_malloc && start_pos[z] != -1) {
                    length_difference = calculate_length_difference(current_token, start_pos[z],info);
                    current_token = expand_variable2(current_token, start_pos[z],info);
                    while (h < size_malloc && start_pos[h] != -1) {
                        if(start_pos[h] + length_difference >= 0) {
                            start_pos[h] += length_difference;
                        }
                        
                        h++;
                    }
                    z++;
                    h = 0;
        }
        z = 0;
    }
            //add_token_to_list(info, current_token, is_first_token ? CMD : (expect_file ? FIL : ARG));
            current_section = append_to_current_section(info, current_section, current_token);
            //my_free(current_token);
            current_token = NULL;
            j = reset_positions(start_pos, size_malloc);
            j = 0;
            quote_state = NONE;
        }
        //is_first_token = 0;
        //expect_file = 0;
        i++;
        continue;  // Saltar el salto de línea para continuar
    }
        
        // Manejo de comillas dobles
        if (section[i] == '\"') {
            if (!in_single_quotes) {        
                in_double_quotes = !in_double_quotes;
                if (in_double_quotes) {
                    quote_state = DOUBLE_QUOTE;
                }
                current_token = add_char_to_token2(info, current_token, section[i]);
                // No cambiar `quote_state` al cerrar comillas dobles
            } else {
                current_token = add_char_to_token2(info, current_token, section[i]);
                if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
                
            }
            i++;
            continue;
        }

        // Manejo de comillas simples
        if (section[i] == '\'') {
            if (!in_double_quotes) {
                in_single_quotes = !in_single_quotes;
                if (in_single_quotes) {
                    quote_state = SINGLE_QUOTE;
                }
                current_token = add_char_to_token2(info, current_token, section[i]);
                // No cambiar `quote_state` al cerrar comillas simples
            } else {
                current_token = add_char_to_token2(info, current_token, section[i]);
                    if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
                
                
            }
            i++;
            continue;
        }

        // Manejo de >> como un token individual
        if (section[i] == '>' && section[i + 1] == '>' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
        
                //add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                current_section = append_to_current_section(info, current_section, current_token);
                //(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            //add_token_to_list(info, ">>", APPEND);
            current_section = append_to_current_section(info, current_section, ">>");
            //my_free(current_token);
            current_token = NULL;
            i++;
            //expect_file = 1;
            //is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            quote_state = NONE;
        }
        // Manejo de > como token individual
        else if (section[i] == '>' && section[i + 1] != '>' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
                //add_to_list(info, current_token, is_first_token ? CMD : ARG);
                current_section = append_to_current_section(info, current_section, current_token);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            //add_token_to_list(info, ">", TRUNC);
            current_section = append_to_current_section(info, current_section, ">");
            //my_free(current_token);
            current_token = NULL;
            //expect_file = 1;
            //is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            quote_state = NONE;
        }
        // Manejo de < como token individual
        else if (section[i] == '<' && section[i + 1] == '<' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
        
                //add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                current_section = append_to_current_section(info, current_section, current_token);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            
            current_section = append_to_current_section(info, current_section, "<<");
            //my_free(current_token);
            i++;
            //expect_file = 1;
            //is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            quote_state = NONE;
        }
        // Manejo de > como token individual
        else if (section[i] == '<' && section[i + 1] != '<' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
                //add_to_list(info, current_token, is_first_token ? CMD : ARG);
                current_section = append_to_current_section(info, current_section, current_token);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            //add_token_to_list(info, ">", TRUNC);
            current_section = append_to_current_section(info, current_section, "<");
            //my_free(current_token);
            current_token = NULL;
            //expect_file = 1;
            //is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            quote_state = NONE;
        }
    
        // Manejo de | solo si no estamos dentro de comillas
        else if (section[i] == '|' && !in_single_quotes && !in_double_quotes) {
            //is_first_token = 1;
            if (current_token) {
        if (quote_state != SINGLE_QUOTE && count_dollars(section)) {
                while (z < size_malloc && start_pos[z] != -1) {
                    length_difference = calculate_length_difference(current_token, start_pos[z],info);
                    current_token = expand_variable2(current_token, start_pos[z],info);
                    while (h < size_malloc && start_pos[h] != -1) {
                        if(start_pos[h] + length_difference >= 0) {
                            start_pos[h] += length_difference;
                        }
                        
                        h++;
       	             }
                    z++;
                    h = 0;
        }
        z = 0;
    }
                //add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                current_section = append_to_current_section(info, current_section, current_token);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            while (section[i] == '|') {
                //add_token_to_list(info, "|", PIPE);
                current_section = append_to_current_section(info, current_section, "|");
                //my_free(current_token);
                current_token = NULL;
                i++;
                //is_first_token = 1;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state = NONE;
            }
            continue;
        }
        // Manejo de espacios o tabuladores si no estamos dentro de comillas
        else if (ft_isspace(section[i]) && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
       // if (quote_state != SINGLE_QUOTE && count_dollars(section)) {
          if (!in_single_quotes && count_dollars(section)) {    
		    while (z < size_malloc && start_pos[z] != -1) {
                    length_difference = calculate_length_difference(current_token, start_pos[z],info);
                    current_token = expand_variable2(current_token, start_pos[z],info);
                    while (h < size_malloc && start_pos[h] != -1) {
                        if(start_pos[h] + length_difference >= 0) {
                            start_pos[h] += length_difference;
                        }
                        
                        h++;
                    }
                    z++;
                    h = 0;
        }
        z = 0;
    }
                //add_token_to_list(info, current_token, is_first_token ? CMD : (expect_file ? FIL : ARG));
                current_section = append_to_current_section(info, current_section, current_token);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                quote_state=NONE;
                //is_first_token = 0;
            }
            
                //is_first_token = 0;
        
            //is_first_token = 0;
            //expect_file = 0;
        }
        else {
            // Añadir carácter a current_token
                current_token = add_char_to_token2(info, current_token, section[i]);
                if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
            
        }

        i++;
    }
    //if (count_dollars(section))
        //print_start_pos(start_pos);
    // Manejo final del token si existe
if (current_token) {
    // Si hay variables de entorno a expandir y no estamos en comillas simples
       // if (quote_state != SINGLE_QUOTE && count_dollars(section)) {
       if (!in_single_quotes && count_dollars(section)) {
		while (z < size_malloc && start_pos[z] != -1) {
                    length_difference = calculate_length_difference(current_token, start_pos[z],info);
                    current_token = expand_variable2(current_token, start_pos[z],info);
                    while (h < size_malloc && start_pos[h] != -1) {
                        if(start_pos[h] + length_difference >= 0) {
                            start_pos[h] += length_difference;
                        }
                        
                        h++;
                    }
                    z++;
                    h = 0;
        }
        z = 0;
    }
    
    // Añadir el último token a la lista
    //add_token_to_list(info, current_token, is_first_token ? CMD : (expect_file ? FIL : ARG));
    current_section = append_to_current_section(info, current_section, current_token);
    //my_free(current_token);
    current_token = NULL;
    j = reset_positions(start_pos, size_malloc);
    j = 0;
    quote_state = NONE;
    //irst_token = 0;
}

// Aquí puedes liberar start_pos si fue utilizado
//if (count_dollars(section))
//    free(start_pos);

trim_trailing_whitespace(current_section);
//free(current_token);
//free(length_difference);
//if (current_token)
    //my_free(current_token);
//if (start_pos != NULL) 
    //my_free(start_pos);
return current_section;    
}
//printf("Tokens concatenados2: %s\n", current_section);



void extract_tokens(const char *section, t_general *info) {
    char *current_token = NULL;  // Única variable para construcción de tokens
    int i = 0;
    int is_first_token = 1;      // Indicador de primer token en la sección
    int expect_file = 0;         // Indicador para identificar si el próximo token es un FILE
    int in_single_quotes = 0;    // Controla el estado de las comillas simples
    int in_double_quotes = 0;    // Controla el estado de las comillas dobles
    //QuoteState quote_state = NONE; // Inicialización de la variable de estado
    int *start_pos = NULL;
    int size_malloc;
    int j = 0;
    

    if (section && count_dollars(section) > 0) {
        size_malloc = count_dollars(section);
        start_pos = my_malloc(info, size_malloc * sizeof(int)); // size_t
        if (start_pos == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        for (int k = 0; k < size_malloc; k++) {
            start_pos[k] = -1;
        }
        //print_start_pos(start_pos);
    }

    
    while (section[i] != '\0') {

         // Manejo del salto de línea ('\n')
    
        // Manejo de comillas dobles
        if (section[i] == '\"') {
            if (!in_single_quotes) {        
                in_double_quotes = !in_double_quotes;
                if (in_double_quotes) {
                    //quote_state = DOUBLE_QUOTE;
                }
                // No cambiar `quote_state` al cerrar comillas dobles
            } else {
                current_token = add_char_to_token(info, current_token, section[i]);
                if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
                
            }
            i++;
            continue;
        }

        // Manejo de comillas simples
        if (section[i] == '\'') {
            if (!in_double_quotes) {
                in_single_quotes = !in_single_quotes;
                if (in_single_quotes) {
                    //quote_state = SINGLE_QUOTE;
                }
                // No cambiar `quote_state` al cerrar comillas simples
            } else {
                current_token = add_char_to_token(info, current_token, section[i]);
                    if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
                
                
            }
            i++;
            continue;
        }

        // Manejo de >> como un token individual
        if (section[i] == '>' && section[i + 1] == '>' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
        
                add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            add_token_to_list(info, ">>", APPEND);
            //my_free(current_token);
            current_token = NULL; 
            i++;
            expect_file = 1;
            is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            //quote_state = NONE;
        }
        // Manejo de > como token individual
        else if (section[i] == '>' && section[i + 1] != '>' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
                add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            add_token_to_list(info, ">", TRUNC);
            //my_free(current_token);
            current_token = NULL;
            expect_file = 1;
            is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            //quote_state = NONE;
        }
        else if (section[i] == '<' && section[i + 1] == '<' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {
        
                add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            add_token_to_list(info, "<<", INPUT);
            //my_free(current_token);
            current_token = NULL; 
            i++;
            expect_file = 1;
            is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            //quote_state = NONE;
        }
        // Manejo de < como token individual
        else if (section[i] == '<' && !in_single_quotes && !in_double_quotes) {
            if (current_token) {

                add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            add_token_to_list(info, "<", INPUT);
            //my_free(current_token);
            current_token = NULL;
            expect_file = 1;
            is_first_token = 0;
            j=reset_positions(start_pos, size_malloc);
            j = 0;
            //quote_state = NONE;
        }
        // Manejo de | solo si no estamos dentro de comillas
        else if (section[i] == '|' && !in_single_quotes && !in_double_quotes) {
            is_first_token = 1;
            if (current_token) {

                add_token_to_list(info, current_token, is_first_token ? CMD : ARG);
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            while (section[i] == '|') {
                add_token_to_list(info, "|", PIPE);
                //my_free(current_token);
                current_token = NULL;
                i++;
                is_first_token = 1;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state = NONE;
            }
            continue;
        }
        // Manejo de espacios o tabuladores si no estamos dentro de comillas
        else if (ft_isspace(section[i]) && !in_single_quotes && !in_double_quotes) {
            if (current_token) {

                add_token_to_list(info, current_token, is_first_token ? CMD : (expect_file ? FIL : ARG));
                //my_free(current_token);
                current_token = NULL;
                j=reset_positions(start_pos, size_malloc);
                j = 0;
                //quote_state=NONE;
               //ES  AQUIIIII   is_first_token = 0;
				if (expect_file == 0)
					is_first_token = 0;
				expect_file = 0;
            }
        
        }
        else {
            // Añadir carácter a current_token
                current_token = add_char_to_token(info, current_token, section[i]);
                if(section[i] == '$' && !in_single_quotes && current_token) {
                    start_pos[j] = strlen(current_token);
                    j++;
                }
            
        }

        i++;
    }
    //if (count_dollars(section))
        //print_start_pos(start_pos);
    // Manejo final del token si existe
if (current_token)
{
    // Añadir el último token a la lista
    //add_token_to_list(info, current_token, is_first_token ? CMD : (expect_file ? FIL : ARG));
    add_token_to_list(info, current_token, expect_file ? FIL : (is_first_token ? CMD : ARG));
	//my_free(current_token);
    current_token = NULL;

    // Resetear posiciones
    j = reset_positions(start_pos, size_malloc);
    j = 0;
    //quote_state = NONE;
    is_first_token = 0;
}

// Aquí puedes liberar start_pos si fue utilizado
//if (start_pos)
//if (start_pos != NULL) 
    //my_free(start_pos);
//if (current_token)
    //my_free(current_token);


}

