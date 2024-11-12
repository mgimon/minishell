/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/12 21:24:24 by mgimon-c         ###   ########.fr       */
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

void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		Global_status = 130;
		write(1, "\n", 1);
		exit(130);
	}
    if (sig == SIGQUIT)
    {
		put_str_fd(2, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b ");
    }
}

void	heredoc_wait(int sig)
{
	if (sig == SIGINT)
		Global_status = 130;
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		Global_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	child_handler(int sig)
{
	if (sig == SIGINT)
	{
		put_str_fd(2, "\n");
		Global_status = 130;
	}
	if (sig == SIGQUIT)
	{
		Global_status = 131;
		put_str_fd(2, "Quit - core dumped \n");
	}
}

// Configura los manejadores de señales
void setup_signals(int i)
{
	if (i == 0)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (i == 1)
	{
		signal(SIGINT, child_handler);
		signal(SIGQUIT, child_handler);
	}
	else if (i == 2)
	{
		signal(SIGINT, heredoc_handler);
		signal(SIGQUIT, heredoc_handler);
	}
	else if (i == 3)
	{
		signal(SIGINT, heredoc_wait);
		signal(SIGQUIT, SIG_IGN);
	}
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
void handle_special_commands(const char *input, int *print_mode) {
    if (strcmp(input, "print_on") == 0) {
        *print_mode = 1;
        printf("Print mode enabled\n");
    } else if (strcmp(input, "print_off") == 0) {
        *print_mode = 0;
        printf("Print mode disabled\n");
    }
}

// Maneja comandos especiales como 'print_on' y 'print_off'
void handle_input(t_general *info, char *input, int *print_mode)
{
    char    *new_input;
    t_token *token_list;
    t_token *current;
        
    add_history(input);
    handle_special_commands(input, print_mode);
	if (!input)
		input="\n";

    while (1)
    {
		new_input = extract_current_section(input, info);
        if (!new_input)
			break ;
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
    init_history(history_file);
    set_paths_and_env(&info, env);
    while (1)
	{
    	setup_signals(0);
        input = readline("mini> ");
		if (Global_status != 0)
			info.exit_status = Global_status;
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
        	if (check_syntax_errors(&info) == 1) {
            // Error encontrado; espera el siguiente prompt
           		free(input);
            	input = NULL;
				continue;
        	}
		//print_tokens_list_alb(&info);
        	info.sections = create_sections_list(&info);
			//print_sections_info(info.sections);
			executor(&info);
			free_heredocs(&info);
			free_all_allocated_blocks(&info);
        	free_sections_list(info.sections);            
        	//free_tokens_list(info.tokens_list);
        	free(input);
			input = NULL;
		}	
		if (input)
		{
			free(input);
			input = NULL;
		}
		else
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
