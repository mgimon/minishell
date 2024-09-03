/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_div_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:19:03 by albealva          #+#    #+#             */
/*   Updated: 2024/09/03 16:41:38 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_plain_text(char **rest, t_general *inf, t_token_flags *flg)
{
	char	*token_start;
	size_t	length;

	token_start = *rest;
	while (**rest && **rest != ' ' && **rest != '"' && **rest != '\'')
		(*rest)++;
	length = *rest - token_start;
	if (length > 0)
		process_token(token_start, rest, inf, flg);
}

void	tokenize_input(t_general *info, char *input)
{
	char			*rest;
	char			*incomplete_token;
	int				previous_redirect;
	int				first_token;
	t_token_flags	flags;

	rest = input;
	incomplete_token = NULL;
	previous_redirect = 0;
	first_token = 1;
	flags.previous_redirect = &previous_redirect;
	flags.first_token = &first_token;
	flags.info = info;
	info->number_of_tokens = 0;
	info->tokens_list = NULL;
	while (*rest)
	{
		p_tok(&rest, info, &flags, &incomplete_token);
		if (incomplete_token)
		{
			process_incomplete_token_content(incomplete_token, info);
			free(incomplete_token);
			break ;
		}
	}
}
