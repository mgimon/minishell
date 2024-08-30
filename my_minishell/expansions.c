/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:49 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/30 23:09:01 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_expansion(t_general *info, char *str)
{
	char	*result;
	char	*status_str;
	char	*rest_str;

	result = NULL;
	if (str[1] == '?')
	{
		rest_str = str;
		status_str = ft_itoa(info->exit_status);
		rest_str += 2;
		result = ft_strjoin(status_str, rest_str);
		free(status_str);
		return (result);
	}
		
	return (result);	
}

void expand_token_string(t_token *tmp, char *status_str, int old_len, int new_len)
{
    char *new_str;
    int i;
    int j;
    int k;

    new_str = malloc(sizeof(char) * (new_len + 1));
    if (!new_str)
        return;

    i = 0;
    j = 0;
    while (i < old_len)
    {
        if (tmp->str[i] == '$' && tmp->str[i + 1] == '?')
        {
            k = 0;
            while (status_str[k])
                new_str[j++] = status_str[k++];
            i += 2;
        }
        else
            new_str[j++] = tmp->str[i++];
    }
    new_str[j] = '\0';
    free(tmp->str);
    tmp->str = new_str;
}

void	expand_tokens_if_status(t_general *info)
{
    t_token *tmp;
    char    *status_str;
    int     i;
	int		j;
	int		flag;

    tmp = info->tokens_list;
    status_str = ft_itoa(info->exit_status);
    while (tmp)
    {
        i = 0;
        j = 0;
        flag = 0;
        while (tmp->str[i])
        {
            if (tmp->str[i] == '$' && tmp->str[i + 1] == '?')
            {
                j += ft_strlen(status_str);
                i += 2;
                flag = 1;
            }
            else
            {
                j++;
                i++;
            }
        }
        if (flag == 1)
			expand_token_string(tmp, status_str, i, j);
        tmp = tmp->next;
    }
    free(status_str);
}

void	expand_tokens_if_variable(t_general *info)
{
    t_token *tmp;
    char *new_str;
    int new_length;
    int original_length;

    tmp = info->tokens_list;
    while (tmp)
    {
        original_length = (int)ft_strlen(tmp->str);
        new_length = calculate_new_length(tmp->str, info->env);

        if (new_length >= original_length)
        {
            new_str = malloc(new_length + 1);
            if (new_str)
            {
                fill_expanded_string(tmp->str, new_str, info->env);
                free(tmp->str);
                tmp->str = new_str;
            }
        }
        tmp = tmp->next;
    }
}

void	expand_expandable_tokens(t_general *info)
{
	expand_tokens_if_variable(info);
	expand_tokens_if_status(info);
}
