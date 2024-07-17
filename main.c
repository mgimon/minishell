/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/17 17:23:48 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void handle_sigint(int sig)
{
    exit(0);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
    char	*input;

    signal(SIGINT, handle_sigint);
    while (1)
	{
        input = readline("mini> ");
        if (input == NULL)
            break; // Salir si readline == EOF (NULL)

        //Checks parseo a full, verificar input OK
		//Si input OK, tokenizar el input
		//Si tokenizacion OK, ejecutar los tokens

        if (input[0] != '\0')
            add_history(input);

        free(input); // Liberar la memoria asignada por readline
    }
    return (0);
}
