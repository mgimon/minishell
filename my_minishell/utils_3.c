/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:21:15 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/16 20:19:23 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_pipex(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

void	set_path(t_section *section)
{
	int	i;

	i = 0;
	if (section->paths)
	{
		while (section->paths[i] != NULL)
		{
			if (access(section->paths[i], R_OK) == 0)
			{
				section->path = ft_strdup(section->paths[i]);
				return ;
			}
			i++;
		}
	}
	section->path = ft_strdup(section->cmdv[0]);
}

void	set_cmd_in_paths(t_section *section)
{
	char	*new_path;
	char	*tmp;
	int		i;

	i = 0;
	if (!section->paths)
		return ;
	if (!section->cmdv || !section->cmdv[0])
		exit(0);
	while (section->paths[i] != NULL)
	{
		tmp = ft_strjoin_pipex(section->paths[i], "/");
		new_path = ft_strjoin_pipex(tmp, section->cmdv[0]);
		free(tmp);
		if (new_path == NULL)
		{
			while (i >= 0)
			{
				free(section->paths[i]);
				i--;
			}
			free(section->paths);
			return ;
		}
		free(section->paths[i]);
		section->paths[i] = new_path;
		new_path = NULL;
		i++;
	}
}

// returns 0 if builtin found and successfully processed
// returns a positive value otherwise
int	exec_if_builtin(t_section *current)
{
	int	output;

	output = 1;
	if (ft_strncmp_pipex(current->cmdv[0], "echo", 4) == 0)
		output = execute_echo(current);
/*	else if (ft_strncmp_pipex(cmdv[0], "cd", 2) == 0)
		output = execute_cd(current);*/
	else if (ft_strncmp_pipex(current->cmdv[0], "pwd", 3) == 0)
		output = execute_pwd(current);
/*	else if (ft_strncmp_pipex(cmdv[0], "export", 6) == 0)
		output = execute_export(current);
	else if (ft_strncmp_pipex(cmdv[0], "unset", 5) == 0)
		output = execute_unset(current);
	else if (ft_strncmp_pipex(cmdv[0], "env", 3) == 0)
		output = execute_env(current);
	else if (ft_strncmp_pipex(cmdv[0], "exit", 4) == 0)
		output = execute_exit(current);*/
	return (output);
}
