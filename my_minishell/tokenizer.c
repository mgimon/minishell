/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:26:31 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/14 20:46:01 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenizer(t_general *info, char *input)
{
	(void)input;
	t_token	*list;

	//----------- hardcodeo -------------------
/*	info->number_of_tokens = 8;
	list = malloc(sizeof(t_token) * info->number_of_tokens);

	list[0].str = "<<";
	list[0].type = 5;
	list[0].prev = NULL;
	list[0].next = &list[1];

    list[1].str = "blabla";
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

    list[4].str = ">";
    list[4].type = 3;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = "outfile";
    list[5].type = 6;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = ">";
    list[6].type = 3;
    list[6].prev = &list[5];
    list[6].next = &list[7];

    list[7].str = "outfile2";
    list[7].type = 6;
    list[7].prev = &list[6];
    list[7].next = NULL;*/

	info->number_of_tokens = 8;
	list = malloc(sizeof(t_token) * info->number_of_tokens);

    list[0].str = "<<";
    list[0].type = 5;
    list[0].prev = NULL;
    list[0].next = &list[1];

    list[1].str = "blabla";
    list[1].type = 6;
    list[1].prev = &list[0];
    list[1].next = &list[2];

    list[2].str = "grep";
    list[2].type = 1;
    list[2].prev = &list[1];
    list[2].next = &list[3];

    list[3].str = "hola";
    list[3].type = 2;
    list[3].prev = &list[2];
    list[3].next = &list[4];

    list[4].str = ">";
    list[4].type = 3;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = "outfile";
    list[5].type = 6;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = ">";
    list[6].type = 3;
    list[6].prev = &list[5];
    list[6].next = &list[7];

    list[7].str = "outfile2";
    list[7].type = 6;
    list[7].prev = &list[6];
    list[7].next = NULL;

/*
	info->number_of_tokens = 7;
	list = malloc(sizeof(t_token) * info->number_of_tokens);

    list[0].str = "<<";
    list[0].type = 5;
    list[0].prev = NULL;
    list[0].next = &list[1];

    list[1].str = "blabla";
    list[1].type = 6;
    list[1].prev = &list[0];
    list[1].next = &list[2];

    list[2].str = "cat";
    list[2].type = 1;
    list[2].prev = &list[1];
    list[2].next = &list[3];

    list[3].str = ">";
    list[3].type = 3;
    list[3].prev = &list[2];
    list[3].next = &list[4];

    list[4].str = "outfile";
    list[4].type = 6;
    list[4].prev = &list[3];
    list[4].next = &list[5];

    list[5].str = ">";
    list[5].type = 3;
    list[5].prev = &list[4];
    list[5].next = &list[6];

    list[6].str = "outfile2";
    list[6].type = 6;
    list[6].prev = &list[5];
    list[6].next = NULL;

*/

	info->tokens_list = list;
}
