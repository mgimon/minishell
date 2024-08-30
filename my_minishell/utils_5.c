/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 21:35:56 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/30 23:09:15 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int calculate_new_length(const char *str, char **env)
{
    int     i;
    int     j;
    int     start;
    int     len;
    char    *var_name;
    char    *value;

	i = 0;
	j = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && str[i + 1] != '$' && str[i + 1] != ' ')
        {
            start = i + 1;
            i++;
            while (str[i] && (str[i] == '_' || ft_isalnum(str[i])))
                i++;
            len = i - start;
            var_name = ft_substr(str, start, len);
            if (var_name)
            {
                value = ft_getenv(var_name, env);
                free(var_name);
                if (value)
                    j += ft_strlen(value);
                //free(value);
            }
        }
        else
        {
            j++;
            i++;
        }
    }
    return j;
}

void fill_expanded_string(const char *src, char *dest, char **env)
{
	int		i;
	int		j;
	int		k;
	int		start;
	int		len;
    char	*var_name;
	char	*value;

	i = 0;
	j = 0;
    while (src[i])
    {
        if (src[i] == '$' && src[i + 1] && src[i + 1] != '$' && src[i + 1] != ' ')
        {
            start = i + 1;
            i++;
            while (src[i] && (src[i] == '_' || ft_isalnum(src[i])))
                i++;
            len = i - start;
            var_name = ft_substr(src, start, len);
            if (var_name)
            {
                value = ft_getenv(var_name, env);
                free(var_name);
                if (value)
                {
                    k = 0;
                    while (value[k])
                        dest[j++] = value[k++];
                   //free(value);
                }
            }
        }
        else
            dest[j++] = src[i++];
    }
    dest[j] = '\0';
}
