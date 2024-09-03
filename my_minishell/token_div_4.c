/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_div_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:17:56 by albealva          #+#    #+#             */
/*   Updated: 2024/09/03 16:41:25 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_incomplete_token(char *token_start,
	char **rest, char **incomplete_token)
{
	size_t	length;

	length = *rest - token_start + 1;
	*incomplete_token = malloc(length + 1);
	if (!*incomplete_token)
		handle_malloc_failure();
	strncpy(*incomplete_token, token_start - 1, length);
	(*incomplete_token)[length] = '\0';
	*rest = token_start + length - 1;
}

void	handle_quotes(char **rest, char *quote, char **token_start)
{
	*quote = **rest;
	(*rest)++;
	*token_start = *rest;
	while (**rest && **rest != *quote)
		(*rest)++;
}

void	process_and_free(char *incomplete_token, t_general *info)
{
	process_incomplete_token_content(incomplete_token, info);
	free(incomplete_token);
}

void	p_tok(char **rest, t_general *inf, t_token_flags *flg, char **inco_tok)
{
	skipespace(rest);
	if (**rest == '\0')
		return ;
	if (**rest == '"' || **rest == '\'')
		handle_quotes_and_process(rest, flg, inco_tok);
	else
		handle_plain_text(rest, inf, flg);
}

void	handle_quotes_and_process(char **rest,
	t_token_flags *flg, char **inco_tok)
{
	char	quote;
	char	*token_start;

	handle_quotes(rest, &quote, &token_start);
	if (**rest == quote)
		process_token(token_start, rest, flg->info, flg);
	else
	{
		handle_incomplete_token(token_start, rest, inco_tok);
		return ;
	}
}
