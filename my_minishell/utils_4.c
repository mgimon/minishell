/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:49:28 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/24 15:29:04 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *add_var_equal(char *cmdv1)
{
    char    *result;
    int     i;

    i = 0; 
    result = malloc((sizeof(char) * ft_strlen(cmdv1)) + 2);
    while (cmdv1[i])
    {   
        result[i] = cmdv1[i];
        i++;
    }
    result[i] = '=';
    i++;
    result[i] = '\0';
    return (result);
}

char    **remove_env_line(t_section *current, int line)
{
    int     i;
    int     j;
    char    **new_env;

    i = 0;
    j = 0;
    while (current->info->env[i])
        i++;
    new_env = (char **)malloc(i * sizeof(char *));
    i = 0;
    while (current->info->env[i])
    {
        if (i != line)
        {   
            new_env[j] = ft_strdup(current->info->env[i]);
			if (!new_env[j])
			{
				while (j > 0)
					free(new_env[--j]);
				free(new_env);
				return (current->info->env);
			}
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    return (new_env);
}

int	is_directory(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0)
	{
        if (S_ISDIR(st.st_mode))
            return (1);
    }
    return (0);
}
