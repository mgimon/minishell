/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_div_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:16:42 by albealva          #+#    #+#             */
/*   Updated: 2024/09/03 16:38:01 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flags(int *previous_redirect,
	int *first_token, const t_token *token)
{
	if (token->type == APPEND || token->type == TRUNC || token->type == INPUT)
	{
		*previous_redirect = 1;
	}
	else if (token->type == PIPE || token->type == END)
	{
		*previous_redirect = 0;
		*first_token = 1;
	}
	else if (token->type == FIL)
	{
		*previous_redirect = 0;
	}
	else if (token->type == CMD)
	{
		*first_token = 0;
	}
}

void	classify_token_type(t_token *token, char *inner_token,
	int *previous_redirect, int *first_token)
{
	if (is_redirection(inner_token))
	{
		update_token_type(token, inner_token);
		set_flags(previous_redirect, first_token, token);
	}
	else if (is_pipe_or_end(inner_token))
	{
		update_token_type(token, inner_token);
		set_flags(previous_redirect, first_token, token);
	}
	else if (*previous_redirect)
	{
		token->type = FIL;
		set_flags(previous_redirect, first_token, token);
	}
	else if (*first_token)
	{
		token->type = CMD;
		set_flags(previous_redirect, first_token, token);
	}
	else
		token->type = ARG;
}

void	insert_token_to_list(t_general *info, t_token *new_token)
{
	new_token->next = info->tokens_list;
	if (info->tokens_list != NULL)
		info->tokens_list->prev = new_token;
	new_token->prev = NULL;
	info->tokens_list = new_token;
	printf("Token: '%s' | Tipo: %d\n", new_token->str, new_token->type);
	info->number_of_tokens++;
}

void	create_and_classify_token(t_general *info, char *inner_token,
	int *previous_redirect, int *first_token)
{
	t_token	*new_token;

	new_token = create_new_token(inner_token);
	classify_token_type(new_token, inner_token, previous_redirect, first_token);
	insert_token_to_list(info, new_token);
}

void	process_token(char *token_start,
	char **rest, t_general *info, t_token_flags *flags)
{
	size_t	length;
	char	*inner_token;

	length = *rest - token_start;
	inner_token = malloc(length + 1);
	if (!inner_token)
		handle_malloc_failure();
	strncpy(inner_token, token_start, length);
	inner_token[length] = '\0';
	*rest += 1;
	create_and_classify_token(info, inner_token,
		flags->previous_redirect, flags->first_token);
}
