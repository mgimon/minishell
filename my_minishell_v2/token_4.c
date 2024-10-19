/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:01:01 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:10:32 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	add_token_to_list(t_general *info, const char *str, int type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		error_malloc();
	new_token->str = strdup(str);
	if (!new_token->str)
		error_strdup();
	new_token->type = type;
	new_token->prev = NULL;
	new_token->next = NULL;
	if (!info->tokens_list)
		info->tokens_list = new_token;
	else
	{
		last = info->tokens_list;
		while (last->next)
			last = last->next;
		last->next = new_token;
		new_token->prev = last;
	}
	info->number_of_tokens++;
}

char	*add_char_to_token(char *token, char c)
{
	int		len;
	char	*new_token;

	if (token)
		len = strlen(token);
	else
		len = 0;
	new_token = malloc(len + 2);
	if (!new_token)
		error_malloc();
	if (token)
	{
		strcpy(new_token, token);
		free(token);
	}
	new_token[len] = c;
	new_token[len + 1] = '\0';
	return (new_token);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\r');
}

char	*get_env_var(t_general *info, const char *var_name)
{
	int		i;
	char	*equal_sign;
	size_t	name_length;

	i = 0;
	while (info->env[i] != NULL)
	{
		equal_sign = strchr(info->env[i], '=');
		if (equal_sign != NULL)
		{
			name_length = equal_sign - info->env[i];
			if (strncmp(info->env[i], var_name, name_length) == 0
				&& name_length == strlen(var_name))
				return (equal_sign + 1);
		}
		i++;
	}
	return ("");
}

int	is_special_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\''
		|| c == '\"' || c == '<' || c == '>' || c == '|' || c == '$');
}
