/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:56:14 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/24 19:32:40 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenizer(t_general *info, char *input)
{
	t_token	*list;

	(void)input;
	//----------- pruebas -------------------
	info->number_of_tokens = 9;
	list = malloc(sizeof(t_token) * info->number_of_tokens);

	list[0].str = "<";
	list[0].type = 5;
	list[0].prev = NULL;
	list[0].next = &list[1];

    list[1].str = "vehicles.txt";
    list[1].type = 2;
    list[1].prev = &list[0];
    list[1].next = &list[2];

    list[2].str = "grep";
    list[2].type = 1;
    list[2].prev = &list[1];
    list[2].next = &list[3];

    list[3].str = "powerboat";
    list[3].type = 2;
    list[3].prev = &list[2];
    list[3].next = &list[4];

    list[4].str = "|";
    list[4].type = 6;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = "wc";
    list[5].type = 1;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = "-w";
    list[6].type = 2;
    list[6].prev = &list[5];
    list[6].next = &list[7];

    list[7].str = ">";
    list[7].type = 3;
    list[7].prev = &list[6];
    list[7].next = &list[8];

    list[8].str = "empty.txt";
    list[8].type = 2;
    list[8].prev = &list[7];
    list[8].next = NULL;

	info->tokens_list = list;
	//----------- end pruebas -------------------
	
	//mallockear y rellenar la lista de tokens
}
