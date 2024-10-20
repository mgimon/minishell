/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:08:03 by albealva          #+#    #+#             */
/*   Updated: 2024/10/15 21:06:52 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*expand_dollar_question(t_general *info)
{
	char	*result;

	result = (char *)malloc(12);
	if (result == NULL)
		error_malloc();
	result = ft_itoa(info->exit_status);
	return (result);
}

int	count_dollars(const char *section)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (section[i] != '\0')
	{
		if (section[i] == '$')
			count++;
		i++;
	}
	return (count);
}

void	print_start_pos(int *start_pos)
{
	int	i;

	i = 0;
	printf("Contenido de start_pos: ");
	while (start_pos[i])
	{
		printf("%d ", start_pos[i]);
		i++;
	}
	printf("\n");
}

int	reset_positions(int *start_pos, int size_malloc)
{
	int	k;

	k = 0;
	if (start_pos == NULL)
		return (-1);
	while (k < size_malloc)
	{
		start_pos[k] = -1;
		k++;
	}
	return (0);
}

char	*initialize_var_name(void)
{
	static char	var_name[256];

	memset(var_name, 0, sizeof(var_name));
	return (var_name);
}
