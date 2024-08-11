/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:05:39 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/11 17:51:47 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_echo(t_section *current)
{
    int i;
    int w;
    int no_newline;

    i = 0;
    no_newline = 1;
    if (!current->cmdv[1])
        return (write(STDOUT_FILENO, "\n", 1), 0);
    if (ft_strcmp(current->cmdv[1], "-n") == 0 && (!current->cmdv[2] || current->cmdv[2][0] == '\0'))
        return (0);
    if (ft_strcmp(current->cmdv[1], "-n") == 0)
    {
        no_newline = 0;
        i = 2;
    }
    else
        i = 1;
    while (current->cmdv[i])
    {
        w = 0;
        while (current->cmdv[i][w])
        {
            write(STDOUT_FILENO, &current->cmdv[i][w], 1);
            w++;
        }
        if (current->cmdv[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (no_newline)
		write(STDOUT_FILENO, "\n", 1);
    return (0);
}

