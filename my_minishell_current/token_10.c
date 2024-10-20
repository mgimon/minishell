/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_10.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:39:30 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 18:55:27 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	add_token_and_cleanup(t_extract *ex, t_general *info,
const char *token, int type)
{
	add_token_to_list(info, token, type);
	free(ex->current_token);
	ex->current_token = NULL;
	ex->j = reset_positions(ex->start_pos, ex->size_malloc);
	ex->j = 0;
	ex->quote_state = NONE;
}

void	handle_pipe_token(t_extract *ex, t_general *info, const char *section)
{
	ex->is_first_token = 1;
	if (ex->current_token)
	{
		handle_variable_expansion(ex, info, section);
		if (ex->is_first_token)
			add_token_and_cleanup(ex, info, ex->current_token, CMD);
		else
			add_token_and_cleanup(ex, info, ex->current_token, ARG);
	}
	while (section[ex->i] == '|')
	{
		add_token_and_cleanup(ex, info, "|", PIPE);
		ex->i++;
		ex->is_first_token = 1;
		ex->expect_file = 0;
	}
}

void	handle_space_token(t_extract *ex, t_general *info, const char *section)
{
	if (ex->current_token)
	{
		handle_variable_expansion(ex, info, section);
		if (ex->expect_file)
			add_token_to_list(info, ex->current_token, FIL);
		else
		{
			if (ex->is_first_token)
				add_token_to_list(info, ex->current_token, CMD);
			else
				add_token_to_list(info, ex->current_token, ARG);
		}
		free(ex->current_token);
		ex->current_token = NULL;
		ex->j = reset_positions(ex->start_pos, ex->size_malloc);
		ex->j = 0;
		ex->quote_state = NONE;
		if (ex->expect_file == 0)
			ex->is_first_token = 0;
		ex->expect_file = 0;
	}
}

void	handle_current_token_update(t_extract *ex, const char *section)
{
	ex->current_token = add_char_to_token(ex->current_token, section[ex->i]);
	if (section[ex->i] == '$' && !ex->in_single_quotes && ex->current_token)
	{
		ex->start_pos[ex->j] = strlen(ex->current_token);
		ex->j++;
	}
}

void	finalize_token(t_extract *ex, t_general *info)
{
	if (ex->expect_file)
		add_token_to_list(info, ex->current_token, FIL);
	else
	{
		if (ex->is_first_token)
			add_token_to_list(info, ex->current_token, CMD);
		else
			add_token_to_list(info, ex->current_token, ARG);
	}
	free(ex->current_token);
	ex->current_token = NULL;
	ex->j = reset_positions(ex->start_pos, ex->size_malloc);
	ex->j = 0;
	ex->quote_state = NONE;
	ex->is_first_token = 0;
	ex->expect_file = 0;
}
