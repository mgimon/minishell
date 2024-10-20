/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:10:59 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 19:38:17 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_tokens(const char *section, t_general *info)
{
	t_extract	ex;

	initialize_extract(&ex);
	ex.start_pos = allocate_and_initialize_positions(section, &ex.size_malloc);
	process_section_2(&ex, info, section);
	if (ex.current_token)
	{
		if (!ex.in_single_quotes && count_dollars(section))
			handle_variable_expansion(&ex, info, section);
		finalize_token(&ex, info);
	}
	cleanup_extract(&ex);
}

void	process_section(char *section, t_general *info)
{
	if (section)
	{
		extract_tokens(section, info);
		free(section);
	}
}

t_token	*tokenize_input(t_general *info, char *input)
{
	t_quote_state	state;
	char			*start;
	char			*section;
	const char		*section_delimiters;

	start = input;
	section_delimiters = "|\n\r";
	info->tokens_list = NULL;
	open_quote(input, &state);
	while (*start)
	{
		section = extract_section(&start, section_delimiters);
		if (section)
			process_section(section, info);
	}
	return (info->tokens_list);
}
