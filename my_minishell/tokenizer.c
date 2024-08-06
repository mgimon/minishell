/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:56:14 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/06 18:32:41 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenizer(t_general *info, char *input)
{
	t_token		*list;

	(void)input;

	//----------- hardcodeo -------------------
/*	info->number_of_tokens = 14;
	list = malloc(sizeof(t_token) * info->number_of_tokens);

	list[0].str = "<";
	list[0].type = 5;
	list[0].prev = NULL;
	list[0].next = &list[1];

    list[1].str = "vehicles.txt";
    list[1].type = 6;
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
    list[4].type = 7;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = "cat";
    list[5].type = 1;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = ">";
    list[6].type = 3;
    list[6].prev = &list[5];
    list[6].next = &list[7];

    list[7].str = "outfile.txt";
    list[7].type = 6;
    list[7].prev = &list[6];
    list[7].next = &list[8];

    list[8].str = ">";
    list[8].type = 3;
    list[8].prev = &list[7];
    list[8].next = &list[9];

	list[9].str = "archivo3.txt";
    list[9].type = 6;
    list[9].prev = &list[8];
    list[9].next = &list[10];

    list[10].str = "|";
    list[10].type = 7;
    list[10].prev = &list[9];
    list[10].next = &list[11];

    list[11].str = "cat";
    list[11].type = 1;
    list[11].prev = &list[10];
    list[11].next = &list[12];

    list[12].str = ">";
    list[12].type = 3;
    list[12].prev = &list[11];
    list[12].next = &list[13];

    list[13].str = "archivo4.txt";
    list[13].type = 6;
    list[13].prev = &list[12];
    list[13].next = NULL;*/

	info->number_of_tokens = 8;
    list = malloc(sizeof(t_token) * info->number_of_tokens);

	list[0].str = "echo";
    list[0].type = 1;
    list[0].prev = NULL;
    list[0].next = &list[1];

    list[1].str = "whatsup";
    list[1].type = 2;
    list[1].prev = &list[0];
    list[1].next = &list[2];

    list[2].str = ">";
    list[2].type = 3;
    list[2].prev = &list[1];
    list[2].next = &list[3];

    list[3].str = "empty.txt";
    list[3].type = 6;
    list[3].prev = &list[2];
    list[3].next = &list[4];

    list[4].str = "|";
    list[4].type = 7;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = "cat";
    list[5].type = 1;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = ">";
    list[6].type = 3;
    list[6].prev = &list[5];
    list[6].next = &list[7];

    list[7].str = "outfile.txt";
    list[7].type = 6;
    list[7].prev = &list[6];
    list[7].next = NULL;


	//----------- end hardcodeo -------------------
	info->tokens_list = list;
}
