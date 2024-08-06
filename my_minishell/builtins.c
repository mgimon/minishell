/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:25:28 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/06 18:45:09 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_echo(t_section *current)
{
	int	i;
	int	w;
	int	line;

	i = 0;
	line = 0;
	if (!current->cmdv[1])
		return (write(STDOUT_FILENO, "\n", 1), 1);
	if (ft_strcmp(current->cmdv[1], "-n") == 0 && (!current->cmdv[2] || current->cmdv[2][0] == '\0'))
		return (1);
	if (ft_strcmp(current->cmdv[1], "-n") == 0)
	{
		line = 1;
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
			printf(" ");
		i++;
	}
	if (line > 0)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}
