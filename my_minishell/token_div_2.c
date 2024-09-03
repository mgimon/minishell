/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_div_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:14:50 by albealva          #+#    #+#             */
/*   Updated: 2024/09/03 16:40:49 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_incomplete_token_content(char *incomplete_token,
	t_general *info)
{
	size_t	len;
	char	*new_token_content;
	char	*src;
	char	*dst;

	len = strlen(incomplete_token);
	new_token_content = malloc(len + 1);
	if (!new_token_content)
		handle_malloc_failure();
	src = incomplete_token;
	dst = new_token_content;
	if (*src == '"' || *src == '\'')
		*dst++ = *src++;
	while (*src)
	{
		if (*src == '"' || *src == '\'')
			*dst++ = *src++;
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	process_incomplete_token(new_token_content, info);
	free(new_token_content);
}

t_token	*create_new_token(char *inner_token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		handle_malloc_failure();
	new_token->str = inner_token;
	return (new_token);
}

int	is_redirection(const char *token_str)
{
	return (strcmp(token_str, ">>") == 0 || strcmp(token_str, ">") == 0
		|| strcmp(token_str, "<") == 0);
}

int	is_pipe_or_end(const char *token_str)
{
	return (strcmp(token_str, "|") == 0 || strcmp(token_str, "&&") == 0
		|| strcmp(token_str, "||") == 0 || strcmp(token_str, "&") == 0);
}

void	update_token_type(t_token *token, const char *token_str)
{
	if (strcmp(token_str, ">>") == 0)
		token->type = APPEND;
	else if (strcmp(token_str, ">") == 0)
		token->type = TRUNC;
	else if (strcmp(token_str, "<") == 0)
		token->type = INPUT;
	else if (strcmp(token_str, "|") == 0)
		token->type = PIPE;
	else if (strcmp(token_str, "&&") == 0 || strcmp(token_str, "||")
		== 0 || strcmp(token_str, "&") == 0)
		token->type = END;
}
