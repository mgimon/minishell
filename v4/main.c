/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/09 19:39:47 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	Global_status = 0;

// Inicializa la estructura general
int init_general(t_general *info, char **env)
{
    info->number_of_tokens = 0;
    info->tokens_list = NULL;
    info->env = env;
    return 0;
}

// Controlador de señal para SIGINT (Ctrl+C)
void sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line(); // Indica que estamos en una nueva línea
		rl_redisplay();
		Global_status = 130;
	}
}

void	sigquit_handler(int signo)
{
	if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		put_str_fd(2, "  \b\b"); // pulls back line to hide "^\"
	}
	if (Global_status == -1)
		put_str_fd(2, "\b\b\b\b\b\b\b\b\b\b\bQuit - core dumped :(\n"); // pulls back line to hide "^\"
}

// Configura los manejadores de señales
void setup_signals(void)
{
    signal(SIGINT, sigint_handler); // Manejo de Ctrl+C
    signal(SIGQUIT, sigquit_handler); // Manejo de Ctrl+'\'
}

// Inicializa el historial de comandos
void init_history(const char *history_file) {
    using_history(); // Habilita el uso del historial
    if (access(history_file, F_OK) == 0) {
        read_history(history_file);
    }
}

// Guarda el historial al final del programa
void cleanup(const char *history_file) {
    write_history(history_file);
}

// Maneja comandos especiales como 'print_on' y 'print_off'
void handle_special_commands(const char *input, int *print_mode) {
    if (strcmp(input, "print_on") == 0) {
        *print_mode = 1;
        printf("Print mode enabled\n");
    } else if (strcmp(input, "print_off") == 0) {
        *print_mode = 0;
        printf("Print mode disabled\n");
    }
}

// Maneja la entrada del usuario y añade comandos al historial
void handle_input(t_general *info, char *input, int *print_mode)
{
    char    *new_input;
    t_token *token_list;
    t_token *current;
        
    add_history(input);
    handle_special_commands(input, print_mode);


    while (1)
    {
        new_input = extract_current_section(input, info);
        if (strcmp(input, new_input) == 0)
        {
            //my_free(new_input);
            //my_free(input);
            break;
        }
        //my_free(input);
        input = new_input;
    }
    token_list = tokenize_input(info, input);
    //free(token_list);
    tokenize_input(info, input);
    if (token_list == NULL) {
        //my_free(input);
        input = NULL;  
        return;
    }
    if (*print_mode)
    {
        current = token_list;
        while (current)
        {
            printf("Token: %s, Type: %s\n", current->str, get_token_type_name(current->type));
           current = current->next;
        }
       printf("Total tokens: %d\n", info->number_of_tokens);
    }
    //my_free(token_list);
    //my_free(input);
    //free(new_input);
    //input = NULL;
    //free_tokens_list(info->tokens_list);
    //free_tokens_list_tokenize(info);
    return ;
}

void print_tokens_list_alb(t_general *info) {
    t_token *current = info->tokens_list;
    int i = 1;

    if (!current) {
        printf("La lista de tokens está vacía.\n");
        return;
    }

    printf("Número de tokens: %d\n", info->number_of_tokens);

    while (current != NULL) {
        printf("Token %d:\n", i);
        printf("  - Valor: %s\n", current->str ? current->str : "(null)");
        printf("  - Tipo: %d\n", current->type);
        printf("  - Anterior: %s\n", current->prev ? current->prev->str : "(null)");
        printf("  - Siguiente: %s\n\n", current->next ? current->next->str : "(null)");

        current = current->next;
        i++;
    }
}

int limtit_input(char *input)
{
    if (input != NULL) {
        if (ft_strlen(input) > 2890)
        {
        
            printf("Error: prompt limit exceeded:\n");
            input[2890] = '\0';  // Truncar la entrada
            //free(input);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv, char **env)
{
    char *input;
    t_general info;
    const char *history_file = ".minishell_history";
    int print_mode = 1;
    t_quote_state state;

    (void)argc;
    (void)argv;
	Global_status = 0;
    init_general(&info, env);
    setup_signals();
    init_history(history_file);
    set_paths_and_env(&info, env);
    while (1)
	{
        input = readline("mini> ");
		if (Global_status != 0)
			info.exit_status = Global_status;
		// intento replicar ctrl+d vs enter(null)
		if (!input)
		{
			
			matrix_free(info.paths); 
			matrix_free(info.exports); 
			matrix_free(info.env);
			put_str_fd(2, "exit\n"); 
			break ;
		}
        if (input && has_content(input) && !limtit_input(input))
		{
        	// Verificar comillas no cerradas antes de manejar la entrada
        	if (open_quote(input, &state) != 0)
			{
            	//printf("Error: Comillas no cerradas\n");
            	free(input);
				input = NULL;
            	continue ; // Esperar la siguiente entrada
        	}
        	handle_input(&info, input, &print_mode);
        	//print_tokens_list_alb(&info);
        	info.sections = create_sections_list(&info);
			//print_sections_info(info.sections);
			executor(&info);
			free_all_allocated_blocks(&info);
        	free_sections_list(info.sections);            
        	//free_tokens_list(info.tokens_list);
        	free(input);
			input = NULL;
		}

		// intento leak clean manu
		if (input)
		{
			free(input);
			input = NULL;
		}
    }

    // Descomentar si deseas guardar el historial
    // cleanup(history_file);
    write_history(history_file);
	free(input);
	input = NULL;
    return (0);
}
