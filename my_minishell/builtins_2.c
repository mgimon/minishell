/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:39:19 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/18 21:06:59 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*transform_var_upperequal(char *cmdv1)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc((sizeof(char) * ft_strlen(cmdv1)) + 2);
	while (cmdv1[i])
	{
		if (cmdv1[i] >= 97 && cmdv1[i] <= 122)
			result[i] = cmdv1[i] - 32;
		else
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
	char	*var_upperequal;
	int		length;
	char	**new_env;

	i = 0;
	if (current->next || !current->cmdv[1] || current->cmdv[1][0] == '\0')
		return ;
	var_upperequal = transform_var_upperequal(current->cmdv[1]);
	length = ft_strlen(var_upperequal);
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], var_upperequal, length) == 0)
		{
			new_env = remove_env_line(current, i);
			current->info->env = new_env;
			matrix_free(current->info->paths);
			current->info->paths = NULL;
		}
		i++;
	}
}
