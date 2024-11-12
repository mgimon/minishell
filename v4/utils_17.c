/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_17.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:50:32 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/09 21:16:44 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

////////////////////////////////--2--///////////////////////////////

int	is_backdirs_only(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != '.' && str[i] != '/')
			return (0);
		if (str[i] == '.')
			count++;
		i++;
	}
	if (count > 3)
		return (count);
	return (0);
}

void	remove_backdirs(t_section *current, int dirs_back)
{
    int i;
    char *pwd_line;
    char *new_pwd;
    char **split_pwd;
    char **new_env;
    int j;
    int new_len;
    int k;
    int env_size;

    i = 0;
	if (dirs_back < 1)
		return ;
    while (current->info->env[i] != NULL)
    {
        if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
            break;
        i++;
    }
    if (current->info->env[i] == NULL)
        return;

    pwd_line = current->info->env[i] + 4;
    if (strlen(pwd_line) == 0)
        return;

    split_pwd = ft_split(pwd_line, '/');
    new_len = 0;
    while (split_pwd[new_len] != NULL)
        new_len++;
    if (dirs_back >= new_len)
        new_len = 0;
    else
        new_len -= dirs_back;

    new_pwd = malloc(5 + 1);
    strcpy(new_pwd, "PWD=");
    for (j = 0; j < new_len; j++)
    {
        size_t needed_size = strlen(new_pwd) + strlen(split_pwd[j]) + 2;
        new_pwd = realloc(new_pwd, needed_size);
        strcat(new_pwd, split_pwd[j]);
        if (j < new_len - 1)
            strcat(new_pwd, "/");
    }

    env_size = 0;
    while (current->info->env[env_size] != NULL)
        env_size++;

    new_env = malloc((env_size + 1) * sizeof(char *));
    k = 0;
    while (k < env_size)
    {
        if (k == i)
            new_env[k] = new_pwd;
        else
            new_env[k] = strdup(current->info->env[k]);
        k++;
    }
    new_env[k] = NULL;

    for (j = 0; current->info->env[j] != NULL; j++)
        free(current->info->env[j]);
    free(current->info->env);
    current->info->env = new_env;

    j = 0;
    while (split_pwd[j] != NULL)
    {
        free(split_pwd[j]);
        j++;
    }
    free(split_pwd);
}

void	if_backdirs_only(t_section *current)
{
	int	i;
	int	dirs_back;

	if (!is_backdirs_only(current->cmdv[1]))
		return ;
	i = is_backdirs_only(current->cmdv[1]);
	dirs_back = (i / 2) - 1;
	if (dirs_back > 1)
		dirs_back--;
	remove_backdirs(current, dirs_back);
}
