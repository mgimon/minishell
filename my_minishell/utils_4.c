/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:49:28 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/30 23:09:12 by mgimon-c         ###   ########.fr       */
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

char *ft_getenv(const char *name, char **env)
{
    char 	prefix[256];
    size_t 	name_len;
    char 	**current;
	size_t	i;

	i = 0;
    if (name == NULL || env == NULL)
        return (NULL);

    name_len = ft_strlen(name);
    if (name_len + 1 >= sizeof(prefix))
        return (NULL);
    while (i < name_len)
	{
        prefix[i] = name[i];
		i++;
	}
    prefix[name_len] = '=';
    prefix[name_len + 1] = '\0';

    current = env;
    while (*current != NULL)
	{
        if (ft_strncmp(*current, prefix, name_len + 1) == 0)
            return (*current + name_len + 1);
        current++;
    }
    return (NULL);
}
