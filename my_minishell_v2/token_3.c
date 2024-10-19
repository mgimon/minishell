/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:58:30 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:15:54 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	error_strdup(void)
{
	perror("Strdup failed");
	exit(EXIT_FAILURE);
}

void	update_quotes(char current_char, int *in_single_quotes,
int *in_double_quotes)
{
	if (current_char == '\'')
		*in_single_quotes = !(*in_single_quotes);
	else if (current_char == '\"')
		*in_double_quotes = !(*in_double_quotes);
}

void	init_extract_section(int *in_single_quotes, int *in_double_quotes)
{
	*in_single_quotes = 0;
	*in_double_quotes = 0;
}

char	*allocate_and_copy_section(char **start, char *end)
{
	size_t	section_length;
	char	*section;

	section_length = end - *start;
	section = malloc(section_length + 1);
	if (!section)
		error_malloc();
	strncpy(section, *start, section_length);
	section[section_length] = '\0';
	*start = end;
	return (section);
}

char	*extract_section(char **start, const char *delimiters)
{
	char	*end;
	char	*section;
	int		in_single_quotes;
	int		in_double_quotes;

	init_extract_section(&in_single_quotes, &in_double_quotes);
	if (**start == '|')
	{
		section = malloc(2);
		if (!section)
			error_malloc();
		section[0] = '|';
		section[1] = '\0';
		(*start)++;
		return (section);
	}
	end = *start;
	while (*end && (!strchr(delimiters, *end)
			|| in_single_quotes || in_double_quotes))
	{
		update_quotes(*end, &in_single_quotes, &in_double_quotes);
		end++;
	}
	section = allocate_and_copy_section(start, end);
	return (section);
}
