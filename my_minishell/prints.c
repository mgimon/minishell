/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:18:36 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/24 17:20:28 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_token_list(t_general *info)
{
    int i;

    i = 0;
    printf("Print token list: \n");
    while (i < info->number_of_tokens)
    {
        printf("%s ", info->tokens_list[i].str);
        i++;
    }
    printf("\n");
}

void    print_matrix(char **matrix)
{
    int i;

    i = 0;
    if (!matrix)
        return ;
    printf("\nPrint matrix: \n");
    while (matrix[i] != NULL)
    {
        printf("%s\n", matrix[i]);
        i++;
    }
}

