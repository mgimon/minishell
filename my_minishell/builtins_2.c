/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:39:19 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/20 22:12:17 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_var_equal(char *cmdv1)
{
	char	*result;
	int		i;

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

char	**remove_env_line(t_section *current, int line)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while (current->info->env[i])
		i++;
	new_env = (char **)malloc((i - 1) * sizeof(char *));
	i = 0;
	while (current->info->env[i])
	{
        if (i != line)
        {
            new_env[j] = ft_strdup(current->info->env[i]);
            j++;
        }
        i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

// unsets from env in t_general
// by creating a env copy without the line to unset (if found)
// additionally, if the unset is PATH, must also free t_general->paths
void	execute_unset(t_section *current)
{
	int		i;
	char	*var_equal;
	int		length;
	char	**new_env;

	i = 0;
	if (current->next || !current->cmdv[1] || current->cmdv[1][0] == '\0')
		return ;
	var_equal = add_var_equal(current->cmdv[1]);
	length = ft_strlen(var_equal);
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], var_equal, length) == 0)
		{
			new_env = remove_env_line(current, i);
			current->info->env = new_env;
			if (ft_strcmp(current->cmdv[1], "PATH") == 0)
				matrix_free(current->info->paths);
			current->info->paths = NULL;
		}
		i++;
	}
}

// THIS SHOULD WORK FOR ALL VARS, not only PATH
char	**remove_paths_env(t_section *current)
{
	int		i;
	int		j;
	int		k;
	int		l;
	char	**result;

    i = 0;
    while (current->info->env[i])
	{
        if (strncmp(current->info->env[i], "PATH=", 5) == 0)
            break;
        i++;
    }
    if (!current->info->env[i])
        return (current->info->env);
    j = 0;
    while (current->info->env[j])
        j++;
    result = malloc((j - 1) * sizeof(char *));
    if (!result)
        return (current->info->env);
    k = 0;
	l = 0;
    while (l < j)
	{
        if (l != i)
		{
            result[k] = strdup(current->info->env[l]);
			if (!result[k])
			{
                matrix_free(result);
                return (current->info->env);
            }
            k++;
        }
		l++;
    }
    result[k] = NULL;
    return (result);
}

// exports to env in t_general
// if the variable is already there, it must reset it to the new variable
// additionally, if the export is PATH, must also update t_general->paths
void	execute_export(t_section *current)
{
	int		i;
	int		j;
	char	**new_env;
	char	**new_paths;
	char	*new_line;

	i = 0;
	j = 0;
	new_paths = NULL;
	if (current->next || !current->cmdv[1] || current->cmdv[1][0] == '\0' || !ft_strchr(current->cmdv[1], '='))
		return ;
	new_line = ft_strdup(current->cmdv[1]);
	if (ft_strncmp_pipex(current->cmdv[1], "PATH=", 5) == 0)
		current->info->env = remove_paths_env(current);
	while (current->info->env[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	while (j < i)
	{
		new_env[j] = current->info->env[j];
		j++;
	}
	new_env[j] = new_line;
	new_env[j + 1] = NULL;
	current->info->env = new_env;
	if (ft_strncmp_pipex(current->cmdv[1], "PATH=", 5) == 0)
		new_paths = ft_split(current->cmdv[1], ':');
	if (new_paths != NULL)
	{
		if (current->info->paths != NULL)
			matrix_free(current->info->paths);
		current->info->paths = new_paths;
	}
}
