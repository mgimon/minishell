/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:22:25 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:14:14 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	copy_initial_chars(const char *input, int start_pos, t_params *p)
{
	int	j;

	j = 0;
	while (j < start_pos - 1)
		p->temp[p->temp_index++] = input[j++];
	if (start_pos == 0)
		p->temp[p->temp_index++] = input[0];
}

int	handle_dollar_question(t_params *p, int *i, t_general *info)
{
	char	*exit_status_str;
	int		len_added;

	len_added = 0;
	exit_status_str = expand_dollar_question(info);
	if (exit_status_str != NULL)
	{
		strcpy(p->temp + p->temp_index, exit_status_str);
		len_added = strlen(exit_status_str);
		p->temp_index += len_added;
		free(exit_status_str);
	}
	(*i) += 1;
	return (len_added);
}

int	handle_expansion_len(const char *input, int *i,
		t_params *p, t_general *info)
{
	static char	var_name[256];
	int			var_index;
	int			len_added;
	char		*value;

	var_index = 0;
	len_added = 0;
	memset(var_name, 0, sizeof(var_name));
	if (input[*i + 1] == '?')
		len_added = handle_dollar_question(p, i, info);
	else
	{
		(*i)++;
		extract_var_name(input, i, var_name, &var_index);
		(*i)--;
		value = get_env_var(info, var_name);
		if (value)
		{
			strcpy(p->temp + p->temp_index, value);
			len_added = strlen(value);
			p->temp_index += len_added;
		}
	}
	return (len_added);
}

void	expand_vars_for_length(const char *input, int start_pos,
		t_params *p, t_general *info)
{
	int	i;
	int	len;

	i = start_pos - 1;
	len = strlen(input);
	while (i < len)
	{
		if (input[i] == '$' && !(p->expanded))
		{
			handle_expansion_len(input, &i, p, info);
			p->expanded = 1;
		}
		else
			p->temp[p->temp_index++] = input[i];
		i++;
	}
}

int	calculate_length_difference(const char *input, int start_pos,
		t_general *info)
{
	int			original_len;
	t_params	p;
	int			expanded_len;

	original_len = strlen(input);
	p.temp = (char *)calloc(1024, sizeof(char));
	p.temp_index = 0;
	p.expanded = 0;
	copy_initial_chars(input, start_pos, &p);
	expand_vars_for_length(input, start_pos, &p, info);
	expanded_len = strlen(p.temp);
	free(p.temp);
	return (expanded_len - original_len);
}
