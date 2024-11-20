/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_24.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:58:22 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/20 20:29:40 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_segmented_pwd(char **split_pwd, int new_len, char *new_pwd)
{
	int		i;
	size_t	needed_size;
	char	*temp;

	i = 0;
	while (i < new_len)
	{
		needed_size = strlen(new_pwd) + strlen(split_pwd[i]) + 2;
		temp = malloc(needed_size);
		strcpy(temp, new_pwd);
		strcat(temp, "/");
		strcat(temp, split_pwd[i]);
		free(new_pwd);
		new_pwd = temp;
		i++;
	}
	return (new_pwd);
}

char	*construct_new_pwd(char **split_pwd, int new_len)
{
	char	*new_pwd;

	new_pwd = malloc(5 + 1);
	if (new_pwd == NULL)
		return (NULL);
	strcpy(new_pwd, "PWD=");
	if (new_len > 0)
		new_pwd = build_segmented_pwd(split_pwd, new_len, new_pwd);
	return (new_pwd);
}

char	**replace_pwd_in_env(char **env, int env_size, int pwd_index,
		char *new_pwd)
{
	char	**new_env;
	int		i;

	new_env = malloc((env_size + 1) * sizeof(char *));
	if (new_env == NULL)
		return (NULL);
	i = 0;
	while (i < env_size)
	{
		if (i == pwd_index)
			new_env[i] = new_pwd;
		else
			new_env[i] = strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	env_size(char **env)
{
	int	size;

	size = 0;
	while (env[size] != NULL)
		size++;
	return (size);
}

void	cleanup_and_replace_env(t_section *current, char **split_pwd,
		char **new_env)
{
	matrix_free(current->info->env);
	matrix_free(split_pwd);
	current->info->env = new_env;
}
