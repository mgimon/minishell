/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/17 21:33:12 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sigint_handler(int signo);
void sigquit_handler(int signo);

void sigint_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\n");
        rl_on_new_line();    // Prepara una nueva línea en la terminal
        rl_replace_line("", 0);  // Limpia la línea en la terminal pero no termina
        rl_redisplay();  // Vuelve a mostrar el prompt al usuario
    } 
    (void)signo;
}

void sigquit_handler(int signo)
{
    if (signo == SIGQUIT)
        printf("Quit\n"); 
    (void)signo;
}

void sigterm_handler(int signo)
{
	if (signo == SIGTERM)
		exit(0);
}

int main(int argc, char **argv, char **env)
{
    char *input;
    t_general info;
    const char *history_file;
	(void)argv;

    history_file = ".minishell_history";
	info.number_of_tokens = 0;
	info.tokens_list = NULL;
	if (argc != 1)
		return (0);
	signal(SIGINT, sigint_handler);   // Para ctrl+C
    signal(SIGQUIT, sigquit_handler); // Para ctrl+"\"
	signal(SIGTERM, sigterm_handler); // Para el builtin exit

    using_history();
    read_history(history_file);

	set_paths_and_env(&info, env);
    while (1)
    {
		input = readline("mini> ");
		if (!input)
		{
			printf("\nExit\n");
			free_info(&info);
			break; // Salida en EOF
		}
		if (*input)
		{
            add_history(input); // Añade al historial si no es una entrada vacía

			//tokenizer(&info, input);
            tokenize_input(&info, input);
			t_token	*new_tokens_list = reverse_copy_list(info.tokens_list);
            free_tokens_list(info.tokens_list);
			info.tokens_list = new_tokens_list;
			//print_tokens_list(info.tokens_list);

			info.sections = create_sections_list(&info);
			//print_sections_info(info.sections);
			executor(&info);

			free_sections_list(info.sections);            
            free_tokens_list(info.tokens_list);
		}
        free(input);
    }
    write_history(history_file);
    return 0;
}
