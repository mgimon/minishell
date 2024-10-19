/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:26:16 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/16 21:15:40 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	handle_special_commands(const char *input, int *print_mode)
{
	if (strcmp(input, "print_on") == 0)
	{
		*print_mode = 1;
		printf("Print mode enabled\n");
	}
	else if (strcmp(input, "print_off") == 0)
	{
		*print_mode = 0;
		printf("Print mode disabled\n");
	}
}

void	handle_input(t_general *info, char *input, int *print_mode)
{
	t_token	*token_list;
	t_token	*current;

	add_history(input);
	handle_special_commands(input, print_mode);
	token_list = tokenize_input(info, input);
	if (token_list == NULL)
	{
		free(input);
		return ;
	}
	if (*print_mode)
	{
		current = token_list;
		while (current)
		{
			printf("Token: %s, Type: %s\n", current->str,
				get_token_type_name(current->type));
			current = current->next;
		}
		printf("Total tokens: %d\n", info->number_of_tokens);
	}
	return ;
}

void	print_token_info(int i, t_token *current)
{
	printf("Token %d:\n", i);
	if (current->str != NULL)
		printf("  - Valor: %s\n", current->str);
	else
		printf("  - Valor: (null)\n");
	printf("  - Tipo: %d\n", current->type);
	if (current->prev != NULL && current->prev->str != NULL)
		printf("  - Anterior: %s\n", current->prev->str);
	else
		printf("  - Anterior: (null)\n");
	if (current->next != NULL && current->next->str != NULL)
		printf("  - Siguiente: %s\n", current->next->str);
	else
		printf("  - Siguiente: (null)\n");
	printf("\n");
}

void	print_tokens_list_alb(t_general *info)
{
	t_token	*current;
	int		i;

	current = info->tokens_list;
	i = 1;
	if (!current)
	{
		printf("La lista de tokens está vacía.\n");
		return ;
	}
	printf("Número de tokens: %d\n", info->number_of_tokens);
	while (current != NULL)
	{
		print_token_info(i, current);
		current = current->next;
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	char			*input;
	t_general		info;
	const char		*history_file;
	int				print_mode;
	t_quote_state	state;

	initialize_program_settings(argc, argv, &history_file, &print_mode);
	initialize_environment(&info, env, history_file);
	while (1)
	{
		input = readline("minitortura> ");
		if (!input)
		{
			printf("\nExit\n");
			break ;
		}
		if (open_quote(input, &state) != 0)
		{
			free(input);
			continue ;
		}
		process_input(&info, input, &print_mode);
	}
	write_history(history_file);
	return (0);
}
