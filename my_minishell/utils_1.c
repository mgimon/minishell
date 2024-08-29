/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:32:56 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/29 20:42:50 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	thereis_pipe(t_token *first)
{
	t_token	*tmp;

	tmp = first;
	if (!tmp)
		return (0);
	while (tmp)
	{
		if (tmp->type == 7)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int count_tokens_per_section(t_token *first)
{
	int		i;
	t_token	*tmp;

	tmp = first;
	if (!tmp || !tmp->str)
		return (0);
	i = 1;
	while (tmp)
	{
		if (tmp->type == 7)
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (i);
}

int count_sections(t_token *first)
{
    t_token *tmp;
    int i;

    tmp = first;
    if (!tmp)
        return (0);
    i = 1;
    while (tmp)
    {
        if (tmp && tmp->type == 7)
            i++;
        tmp = tmp->next;
    }
    return (i);
}

int	ft_strncmp_pipex(const char *str, const char *str2, size_t c)
{
	size_t			i;
	unsigned int	diff;

	diff = 0;
	i = 0;
	while ((str[i] != '\0') && (str2[i] != '\0')
		&& (diff == 0) && (i < c))
	{
		diff = (unsigned char)str[i] - (unsigned char)str2[i];
		i++;
	}
	if ((diff == 0) && (i < c))
		diff = (unsigned char)str[i] - (unsigned char)str2[i];
	return (diff);
}

void	set_paths_and_env(t_general *info, char **env)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	info->exit_status = 0;
	while (env[i])
		i++;
	info->env = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (env[i])
	{
		info->env[i] = ft_strdup(env[i]);
		i++;
	}
	info->env[i] = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp_pipex(env[i], "PATH=", 5) == 0)
		{
			tmp = env[i];
			break ;
		}
		i++;
	}
	i = 0;
	if (tmp)
	{
		info->paths = ft_split(tmp, ':');
		if (info->paths == NULL)
			return ;
	}
}
