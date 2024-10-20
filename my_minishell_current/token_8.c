/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:28:56 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 18:54:27 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	*allocate_and_initialize_positions(const char *section,
int *size_malloc)
{
	int	*start_pos;
	int	i;

	start_pos = NULL;
	i = 0;
	if (section && count_dollars(section) > 0)
	{
		*size_malloc = count_dollars(section);
		start_pos = malloc(*size_malloc * sizeof(int));
		if (start_pos == NULL)
		{
			fprintf(stderr, "Error allocating memory\n");
			exit(EXIT_FAILURE);
		}
		while (i < *size_malloc)
		{
			start_pos[i] = -1;
			i++;
		}
	}
	return (start_pos);
}

void	handle_double_quotes(t_extract *ex, const char *section)
{
	if (section[ex->i] == '\"')
	{
		if (!ex->in_single_quotes)
		{
			ex->in_double_quotes = !ex->in_double_quotes;
			if (ex->in_double_quotes)
				ex->quote_state = DOUBLE_QUOTE;
		}
		else
		{
			ex->current_token = add_char_to_token(ex->current_token,
					section[ex->i]);
			if (section[ex->i] == '$' && !ex->in_single_quotes
				&& ex->current_token)
			{
				ex->start_pos[ex->j] = strlen(ex->current_token);
				ex->j++;
			}
		}
		ex->i++;
	}
}

void	handle_single_quotes(t_extract *ex, const char *section)
{
	if (section[ex->i] == '\'')
	{
		if (!ex->in_double_quotes)
		{
			ex->in_single_quotes = !ex->in_single_quotes;
			if (ex->in_single_quotes)
				ex->quote_state = SINGLE_QUOTE;
		}
		else
		{
			ex->current_token = add_char_to_token(ex->current_token,
					section[ex->i]);
			if (section[ex->i] == '$' && !ex->in_single_quotes
				&& ex->current_token)
			{
				ex->start_pos[ex->j] = strlen(ex->current_token);
				ex->j++;
			}
		}
		ex->i++;
	}
}

void	handle_append_redirect(t_extract *ex, t_general *info)
{
	if (ex->current_token)
	{
		if (ex->is_first_token)
			add_token_to_list(info, ex->current_token, CMD);
		else
			add_token_to_list(info, ex->current_token, ARG);
		free(ex->current_token);
		ex->current_token = NULL;
		ex->j = reset_positions(ex->start_pos, ex->size_malloc);
		ex->j = 0;
		ex->quote_state = NONE;
	}
	add_token_to_list(info, ">>", APPEND);
	free(ex->current_token);
	ex->current_token = NULL;
	ex->i++;
	ex->expect_file = 1;
	ex->j = reset_positions(ex->start_pos, ex->size_malloc);
	ex->j = 0;
	ex->quote_state = NONE;
}

void	handle_trunc_redirect(t_extract *ex, t_general *info)
{
	if (ex->current_token)
	{
		if (ex->is_first_token)
			add_token_to_list(info, ex->current_token, CMD);
		else
			add_token_to_list(info, ex->current_token, ARG);
		free(ex->current_token);
		ex->current_token = NULL;
		ex->j = reset_positions(ex->start_pos, ex->size_malloc);
		ex->j = 0;
		ex->quote_state = NONE;
	}
	add_token_to_list(info, ">", TRUNC);
	ex->expect_file = 1;
	ex->j = reset_positions(ex->start_pos, ex->size_malloc);
	ex->j = 0;
	ex->quote_state = NONE;
}
