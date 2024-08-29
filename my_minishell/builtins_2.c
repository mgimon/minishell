/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:39:19 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/29 20:44:47 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
			if (new_env)
			{
				free(var_equal);
				matrix_free(current->info->env);
				current->info->env = new_env;
				if (ft_strcmp(current->cmdv[1], "PATH") == 0)
				{
					matrix_free(current->info->paths);
					current->info->paths = NULL;
				}
			}
			else
				free(var_equal);
			break ;
		}
		i++;
	}
}

// Returns a copy of env
// without one variable if it matches the users' export request
// Otherwise, returns env
char	**if_remove_var_env(t_section *current)
{
	int		i;
	int		j;
	int		k;
	int		l;
	char	**result;

    k = 0;
	while (current->cmdv[1][k] && current->cmdv[1][k] != '=')
		k++;
	i = 0;
    while (current->info->env[i])
	{
        if (strncmp(current->info->env[i], current->cmdv[1], (k + 1)) == 0)
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
	matrix_free(current->info->env);
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
	current->info->env = if_remove_var_env(current);
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
	free(current->info->env);
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

char	**new_wd_environment(char **oldenv, const char *var_newpwd, const char *var_newoldpwd)
{
	int	i;
    int env_size;
    char **newenv;
    
	env_size = 0;
	i = 0;
    while (oldenv[env_size] != NULL)
        env_size++; 
    newenv = malloc((env_size + 1) * sizeof(char *));
    if (newenv == NULL)
        return NULL;
    while (i < env_size)
	{
        if (strncmp(oldenv[i], "PWD=", 4) == 0)
            newenv[i] = strdup(var_newpwd);
        else if (strncmp(oldenv[i], "OLDPWD=", 7) == 0)
            newenv[i] = strdup(var_newoldpwd);
        else
            newenv[i] = strdup(oldenv[i]);

        if (newenv[i] == NULL)
		{
            while (--i >= 0)
                free(newenv[i]);
            free(newenv);
            return (NULL);
        }
		i++;
    }
    newenv[env_size] = NULL;
    return (newenv);
}

// only absolute or relative routes
void	execute_cd(t_section *current)
{
	int		i;
	char	**new_env;
	char	*var_pwd;
	char	*var_oldpwd;

	var_oldpwd = NULL;
	i = 0;
	if (current->next || !current->cmdv[1])
		return ;
	if (!is_directory(current->cmdv[1]))
	{
		put_str_fd(2, "cd: no such file or directory: ");
		put_str_fd(2, current->cmdv[1]);
		write(2, "\n", 1);
		current->info->exit_status = 1;
		return ;
	}
	if (chdir(current->cmdv[1]) == -1)
		exit(1);
	var_pwd = ft_strjoin("PWD=", current->cmdv[1]);
	i = 0;
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
			var_oldpwd = ft_strjoin("OLD", current->info->env[i]);
		i++;
	}
	new_env = new_wd_environment(current->info->env, var_pwd, var_oldpwd);
	matrix_free(current->info->env);
	free(var_pwd);
	if (var_oldpwd)
		free(var_oldpwd);
	current->info->env = new_env;
}
