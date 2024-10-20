/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:52:22 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 18:56:23 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	handle_quotes(t_extract *ex, const char *section)
{
	handle_double_quotes(ex, section);
	handle_single_quotes(ex, section);
}

void	cleanup_extract(t_extract *ex)
{
	if (ex->start_pos != NULL)
	{
		free(ex->start_pos);
		ex->start_pos = NULL;
	}
	if (ex->current_token != NULL)
	{
		free(ex->current_token);
		ex->current_token = NULL;
	}
}

void	aux(t_extract *ex, t_general *info)
{
	process_current_token(ex, info);
	handle_append_redirect(ex, info);
}

void	process_section_2(t_extract *ex, t_general *info, const char *section)
{
	while (section[ex->i] != '\0')
	{
		handle_quotes(ex, section);
		if (section[ex->i] == '>' && section[ex->i + 1] == '>'
			&& !ex->in_single_quotes && !ex->in_double_quotes)
		{
			aux(ex, info);
			ex->i++;
		}
		else if (section[ex->i] == '>' && section[ex->i + 1] != '>'
			&& !ex->in_single_quotes && !ex->in_double_quotes)
			handle_trunc_redirect(ex, info);
		else if (section[ex->i] == '<' && !ex->in_single_quotes
			&& !ex->in_double_quotes)
			handle_input_redirect(ex, info, section);
		else if (section[ex->i] == '|' && !ex->in_single_quotes
			&& !ex->in_double_quotes)
			handle_pipe_token(ex, info, section);
		else if (ft_isspace(section[ex->i]) && !ex->in_single_quotes
			&& !ex->in_double_quotes)
			handle_space_token(ex, info, section);
		else
			handle_current_token_update(ex, section);
		ex->i++;
	}
}

void	initialize_extract(t_extract *ex)
{
	ex->current_token = NULL;
	ex->i = 0;
	ex->is_first_token = 1;
	ex->expect_file = 0;
	ex->in_single_quotes = 0;
	ex->in_double_quotes = 0;
	ex->quote_state = NONE;
	ex->start_pos = NULL;
	ex->size_malloc = 0;
	ex->j = 0;
	ex->z = 0;
	ex->length_difference = 0;
	ex->h = 0;
}
