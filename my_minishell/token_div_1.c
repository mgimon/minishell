/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_div_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:48:12 by albealva          #+#    #+#             */
/*   Updated: 2024/09/03 16:38:26 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_malloc_failure(void)
{
	perror("Malloc failed");
	exit(EXIT_FAILURE);
}

void	create_and_store_token(char *token_start, char *rest, t_general *info)
{
	size_t	length;
	char	*inner_token;
	t_token	*new_token;

	length = rest - token_start;
	if (length > 0)
	{
		inner_token = malloc(length + 1);
		if (!inner_token)
			handle_malloc_failure();
		strncpy(inner_token, token_start, length);
		inner_token[length] = '\0';
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			handle_malloc_failure();
		new_token->str = inner_token;
		new_token->type = ARG;
		new_token->next = info->tokens_list;
		if (info->tokens_list != NULL)
			info->tokens_list->prev = new_token;
		new_token->prev = NULL;
		info->tokens_list = new_token;
		printf("Token: '%s' | Tipo: %d\n", new_token->str, new_token->type);
		info->number_of_tokens++;
	}
}

void	process_incomplete_token(char *token, t_general *info)
{
	char	*rest;
	char	*token_start;

	rest = token;
	while (*rest)
	{
		while (*rest == ' ')
			rest++;
		if (*rest == '\0')
			break ;
		token_start = rest;
		while (*rest && *rest != ' ')
			rest++;
		create_and_store_token(token_start, rest, info);
	}
}

void	free_tokens_list(t_general *info)
{
	t_token	*current;
	t_token	*next;

	current = info->tokens_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	info->tokens_list = NULL;
	info->number_of_tokens = 0;
}

void	skipespace(char **rest)
{
	while (**rest == ' ')
		(*rest)++;
}
