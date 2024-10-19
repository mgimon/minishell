/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_9.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:35:34 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:14:36 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	reset_ex_values(t_extract *ex)
{
	ex->expect_file = 1;
	ex->j = reset_positions(ex->start_pos, ex->size_malloc);
	ex->j = 0;
	ex->quote_state = NONE;
}

void	handle_input_token(t_extract *ex, t_general *info)
{
	process_current_token(ex, info);
	add_token_to_list(info, "<", INPUT);
	reset_ex_values(ex);
}

void	handle_input_redirect(t_extract *ex, t_general *info,
		const char *section)
{
	if (ex->current_token)
	{
		if (ex->quote_state != SINGLE_QUOTE && count_dollars(section))
		{
			while (ex->z < ex->size_malloc && ex->start_pos[ex->z] != -1)
			{
				ex->length_difference = calculate_length_difference(
						ex->current_token, ex->start_pos[ex->z], info);
				ex->current_token = expand_variable(
						ex->current_token, ex->start_pos[ex->z], info);
				ex->h = 0;
				while (ex->h < ex->size_malloc && ex->start_pos[ex->h] != -1)
				{
					if (ex->start_pos[ex->h] + ex->length_difference >= 0)
						ex->start_pos[ex->h] += ex->length_difference;
					ex->h++;
				}
				ex->z++;
			}
		}
	}
	process_current_token(ex, info);
	add_token_to_list(info, "<", INPUT);
	reset_ex_values(ex);
}

void	process_current_token(t_extract *ex, t_general *info)
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
}

void	handle_variable_expansion(t_extract *ex, t_general *info,
const char *section)
{
	int	z;
	int	h;

	z = 0;
	if (ex->quote_state != SINGLE_QUOTE && count_dollars(section))
	{
		while (z < ex->size_malloc && ex->start_pos[z] != -1)
		{
			ex->length_difference = calculate_length_difference(
					ex->current_token, ex->start_pos[z], info);
			ex->current_token = expand_variable(
					ex->current_token, ex->start_pos[z], info);
			h = 0;
			while (h < ex->size_malloc && ex->start_pos[h] != -1)
			{
				if (ex->start_pos[h] + ex->length_difference >= 0)
					ex->start_pos[h] += ex->length_difference;
				h++;
			}
			z++;
		}
	}
}
