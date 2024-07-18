/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/18 14:36:06 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
	(void)sig;
    exit(0);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
    char	*input;

    signal(SIGINT, handle_sigint);
    while (1)
	{
        input = readline("mini> ");
        if (input == NULL)
            break; // Salir si readline == EOF (NULL)

        //Checks parseo a full, verificar input OK
		//Tokenizamos el input
		tokenizer(input);
		//Si tokenizacion OK, ejecutar los tokens

        if (input[0] != '\0')
            add_history(input);

        free(input); // Liberar la memoria asignada por readline
    }
    return (0);
}
