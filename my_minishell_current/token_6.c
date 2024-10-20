/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:15:51 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:13:45 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	extract_var_name(const char *input, int *i, char *var_name,
int *var_index)
{
	while (input[*i] && !is_special_separator(input[*i])
		&& (isalnum(input[*i]) || input[*i] == '_'))
	{
		var_name[(*var_index)++] = input[*i];
		(*i)++;
	}
	var_name[*var_index] = '\0';
}

char	*handle_dollar_expansion(const char *input,
int *i, t_params *p, t_general *info)
{
	static char	var_name[256];
	int			var_index;
	char		*exit_status_str;

	var_index = 0;
	memset(var_name, 0, sizeof(var_name));
	if (input[*i + 1] == '?')
	{
		exit_status_str = expand_dollar_question(info);
		if (exit_status_str != NULL)
		{
			strcpy(p->temp + p->temp_index, exit_status_str);
			p->temp_index += strlen(exit_status_str);
			free(exit_status_str);
		}
		(*i) += 1;
		return (NULL);
	}
	else
	{
		(*i)++;
		extract_var_name(input, i, var_name, &var_index);
		(*i)--;
		return (get_env_var(info, var_name));
	}
}

void	copy_until_start_pos(const char *input, int start_pos, t_params *p)
{
	int	j;

	j = 0;
	while (j < start_pos - 1)
	{
		p->temp[p->temp_index++] = input[j++];
	}
	if (start_pos == 0)
		p->temp[p->temp_index++] = input[0];
}

void	expand_variables_in_input(const char *input,
	int start_pos, t_params *p, t_general *info)
{
	int		len;
	int		expanded;
	int		i;
	char	*value;

	len = strlen(input);
	expanded = 0;
	i = start_pos - 1;
	while (i < len)
	{
		if (input[i] == '$' && !expanded)
		{
			value = handle_dollar_expansion(input, &i, p, info);
			if (value)
			{
				strcpy(p->temp + p->temp_index, value);
				p->temp_index += strlen(value);
			}
			expanded = 1;
		}
		else
			p->temp[p->temp_index++] = input[i];
		i++;
	}
}

char	*expand_variable(const char *input, int start_pos, t_general *info)
{
	char		*result;
	t_params	p;

	p.temp = (char *)malloc(1024 * sizeof(char));
	p.temp_index = 0;
	memset(p.temp, 0, 1024);
	copy_until_start_pos(input, start_pos, &p);
	expand_variables_in_input(input, start_pos, &p, info);
	result = strdup(p.temp);
	free(p.temp);
	return (result);
}
