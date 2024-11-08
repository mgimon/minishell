/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_16.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:25:03 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/30 12:38:30 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_newline_flag(char **cmdv, int *no_newline)
{
	int	i;

	i = 1;
	if (!cmdv || !cmdv[0] || !cmdv[1])
		return ;
	*no_newline = 1;
	while (cmdv[i + 1] && ft_strcmp(cmdv[i], "-n") == 0)
		i++;
	if (!cmdv[i + 1] && i > 1)
		*no_newline = 0;
}

char	*give_varname(char *str)
{
	int		i;
	int		j;
	char	*varname;

	i = 11;
	j = 0;
	if (!str || !str[i])
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	varname = malloc(sizeof(char) * (i - 11 + 1));
	i = 11;
	while (str[i] && str[i] != '=')
	{
		varname[j] = str[i];
		i++;
		j++;
	}
	varname[j] = '\0';
	return (varname);
}

int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**reset_env(char ***env, int i)
{
	char	**new_env;
	int		j;
	int		k;
	int		len;

	len = count_env(*env);
	new_env = malloc(sizeof(char *) * len);
	if (!new_env)
		return (NULL);
	j = 0;
	k = 0;
	while ((*env)[j])
	{
		if (j != i)
		{
			new_env[k] = ft_strdup((*env)[j]);
			k++;
		}
		j++;
	}
	new_env[k] = NULL;
	return (new_env);
}

void clean_repeateds(char ***env)
{
	int		i;
	int		j;
	int		k;
	int		l;
	char	*s1;
	char	*s2;
	char	**new_env;

	i = 0;
	while ((*env)[i])
	{
		s1 = give_varname((*env)[i]);
		j = i + 1;
		while ((*env)[j])
		{
			s2 = give_varname((*env)[j]);
			if (s1 && s2 && ft_strcmp(s1, s2) == 0)
			{
				free((*env)[i]);
				(*env)[i] = NULL;
				free(s2);
				break ;
			}
			if (s2)
				free(s2);
			j++;
		}
		free(s1);
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return ;
	k = 0;
	l = 0;
	while (l < i)
	{
		if ((*env)[l] != NULL)
		{
			new_env[k] = (*env)[l];
			k++;
		}
		l++;
	}
	new_env[k] = NULL;
	free(*env);
	*env = new_env;
}
