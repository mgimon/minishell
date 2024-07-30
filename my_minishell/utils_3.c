/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:21:15 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/29 17:32:17 by mgimon-c         ###   ########.fr       */
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

// if PATH= is unset, path is first cmd token in prompt
void	set_correct_path(t_section *section)
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
	int		i;

	i = 0;
	if (!section->paths || !section->cmdv)
		return ;
	while (section->paths[i] != NULL)
	{
		new_path = ft_strjoin_pipex(section->paths[i], "/");
		new_path = ft_strjoin_pipex(new_path, section->cmdv[0]);
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
