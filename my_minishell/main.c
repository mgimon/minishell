/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/29 16:01:25 by mgimon-c         ###   ########.fr       */
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
    char		*input;
	t_general	info;

    signal(SIGINT, handle_sigint);
    while (1)
	{
        input = readline("mini> ");
        if (input == NULL)
            break; // Salir si readline == EOF (NULL)

        // Checks parseo a full
		// Las rutas absolutas deben marcarse como un unico token de tipo cmd(1)
		
		// Seteo el path y env en la struct t_general info
		set_path_and_env(&info, env);

		// Hardcodeado de momento
		tokenizer(&info, input);

/* -------------@mgimon-c hace esta parte ---------------------------- */

		//Ejecutar los tokens
		//tokens_executor(&info);

		//Free structs

        if (input[0] != '\0')
            add_history(input);

        free(input); // Liberar la memoria asignada por readline
    }
    return (0);
}
