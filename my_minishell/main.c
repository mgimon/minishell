/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/31 16:26:39 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" // Asegúrate de que este archivo tiene las definiciones para t_general y t_token, más las firmas de funciones mencionadas.
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototipos de las funciones si no están definidos en minishell.h
void sigint_handler(int signo);
void sigquit_handler(int signo);
int init_general(t_general *info, char **env);
void free_tokens_list(t_general *info);
void tokenize_input(t_general *info, char *input);

void sigint_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\n");
        rl_on_new_line();    // Prepara una nueva línea en la terminal
        rl_replace_line("", 0);  // Limpia la línea en la terminal pero no termina
        rl_redisplay();  // Vuelve a mostrar el prompt al usuario
    }
    
    // Silencia el aviso de parámetro no utilizado
    (void)signo;
}

void sigquit_handler(int signo)
{
    if (signo == SIGQUIT)
    {
        printf("Quit: %d\n", signo);  // Muestra el mensaje pero sin finalizar el proceso
    }
    
    // Silencia el aviso de parámetro no utilizado
    (void)signo;
}

int init_general(t_general *info, char **env)
{
    // Se asume que esta función inicializa 'info' y se puede ajustar según 'env'
    info->number_of_tokens = 0;
    info->tokens_list = NULL;
    
    // Envío a callar la advertencia sobre el parámetro 'env' no utilizado al principio
    (void)env;

    return 0; // Retorna 0, que indica éxito
}

int main(int argc, char **argv, char **env)
{
    char *input;
    t_general info; // Declaración de la estructura necesaria
    const char *history_file = ".minishell_history";

    // Inicialización, ignorando parámetros por ahora
    (void)argc;
    (void)argv;
    init_general(&info, env);

    // Configuración del manejo de señales
    signal(SIGINT, sigint_handler);   // Para ctrl+C
    signal(SIGQUIT, sigquit_handler); // Para ctrl+"\"

    using_history();
    read_history(history_file);

    while (1)
    {
        input = readline("mini> ");
        if (!input)
        {
            printf("\nExit\n");
            break; // Salida en EOF
        }
		input = "hey";        
        if (*input)
        {
            //add_history(input); // Añade al historial si no es una entrada vacía

            //tokenize_input(&info, input); // Trata tokenizar la entrada
			set_path_and_env(&info, env);
			tokenizer(&info, input);
			info.sections = create_sections_list(&info);
			print_sections_info(info.sections);
			executor(&info);
            
            //printf("Total tokens: %d\n", recalculate_tokens(&info)); // Muestra el número recalculado de tokens

            // No olvides liberar cada lista de tokens creados aquí.
            //free_tokens_list(&info);
        }

        //free(input); // Liberar el input después de su uso
    }

    // Conservar el historial de comandos para futuras sesiones
    //write_history(history_file);
    return 0;
}
