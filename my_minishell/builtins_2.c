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

char *get_var_name(const char *var)
{
    char *name;
    size_t len = 0;

    while (var[len] && var[len] != '=')
        len++;
    name = (char *)malloc(len + 1);
    if (name)
    {
        ft_memcpy(name, var, len);
        name[len] = '\0';
    }
    return name;
}

int compare_var_name(const char *env_var, const char *var_name)
{
    while (*env_var && *env_var != '=' && *var_name && *env_var == *var_name)
    {
        env_var++;
        var_name++;
    }
    return (*env_var == '=' && *var_name == '\0');
}



void	if_remove_var_from_env(char ***env, char *var)
{
    char **new_env;
    char *var_name;
    size_t env_len = 0;
    size_t i, j;
    int found = 0;

    if (!env || !*env || !var)
        return;
    var_name = get_var_name(var);
    if (!var_name)
        return;
    while ((*env)[env_len])
        env_len++;
    i = 0;
    while (i < env_len)
    {
        if (compare_var_name((*env)[i], var_name))
        {
            found = 1;
            break;
        }
	i++;
    }
    if (!found)
    {
        free(var_name);
        return;
    }
    new_env = (char **)malloc(sizeof(char *) * env_len);
    j = 0;
    i = 0;
    while (i < env_len)
    {
        if (!compare_var_name((*env)[i], var_name))
            new_env[j++] = (*env)[i];
        else
            free((*env)[i]);
	i++;
    }
    new_env[j++] = ft_strdup(var);
    if (!new_env[j - 1])
    {
        free(var_name);
        free(new_env);
        return;
    }
    new_env[j] = NULL;
    free(var_name);
    free(*env);
    *env = new_env;
}


// exports to env in t_general
// if the variable is already there, it must reset it to the new variable
// additionally, if the export is PATH, must also update t_general->paths
void	execute_export(t_section *current)
{
	char	**new_paths;
	int		n;

	n = 1;
	new_paths = NULL;
	if (current->next)
		return ;
	if (!current->cmdv[1])
	{
		print_matrix(current->info->exports);
		return ;
	}
	while (current->cmdv[n])
	{
		if (current->cmdv[n][0] >= '0' && current->cmdv[n][0] <= '9')
			printf("export: %s: not a valid identifier\n", current->cmdv[n]);
		else
		{
			add_str_to_matrix(&current->info->exports, ft_strjoin("declare -x ", current->cmdv[n]));
			if (ft_strchr(current->cmdv[n], '='))
			{
				if_remove_var_from_env(&current->info->env, current->cmdv[n]);
				add_str_to_matrix(&current->info->env, ft_strdup(current->cmdv[n]));
				if (ft_strncmp_pipex(current->cmdv[n], "PATH=", 5) == 0)
				{
					if (new_paths != NULL)
						matrix_free(new_paths);
					new_paths = ft_split(current->cmdv[n], ':');
				}
				if (new_paths != NULL)
				{
					if (current->info->paths != NULL)
						matrix_free(current->info->paths);
					current->info->paths = new_paths;
				}
			}
		}
		n++;
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

char *get_prev_dir(char **env)
{
    char	*pwd;
    char	*prev_dir;
    size_t	len;
    size_t	i;
    size_t	k;
    int		j;

    pwd = NULL;
    j = 0;
    while (env[j] != NULL)
    {
        if (ft_strncmp(env[j], "PWD=", 4) == 0)
	{
            pwd = env[j] + 4;
            break;
        }
	j++;
    }
    if (pwd == NULL)
        return (NULL);
    len = ft_strlen(pwd);
    if (len == 0 || (len == 1 && pwd[0] == '/'))
    {
        prev_dir = (char *)malloc(2 * sizeof(char));
        prev_dir[0] = '.';
        prev_dir[1] = '\0';
        return (prev_dir);
    }
    i = len;
    while (i > 0)
    {
        if (pwd[i - 1] == '/')
            break;
	i--;
    }
    if (i == 0)
    {
        prev_dir = (char *)malloc(2 * sizeof(char));
        prev_dir[0] = '.';
        prev_dir[1] = '\0';
        return (prev_dir);
    }
    prev_dir = (char *)malloc((i + 1) * sizeof(char));
    k = 0;
    while (k < i)
    {
        prev_dir[k] = pwd[k];
	k++;
    }
    prev_dir[i] = '\0';
    return (prev_dir);
}

char *get_home(char **env)
{
    char	*home_value;
    int		i;
    size_t	j;

    i = 0;
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], "HOME=", 5) == 0)
	{
            home_value = (char *)malloc(ft_strlen(env[i] + 5) + 1);
            j = 0;
            while (env[i][j + 5] != '\0')
	    {
                home_value[j] = env[i][j + 5];
		j++;
            }
            home_value[j] = '\0';
            return (home_value);
        }
	i++;
    }
    return (NULL);
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
	if (current->cmdv[2])
		put_str_fd(2, "cd: too many arguments\n");
	if (current->next || current->cmdv[2])
		return ;
	if (current->cmdv[1])
	{
		if (!is_directory(current->cmdv[1]) && ft_strcmp(current->cmdv[1], "~") != 0)
		{
			put_str_fd(2, "cd: no such file or directory: ");
			put_str_fd(2, current->cmdv[1]);
			write(2, "\n", 1);
			current->info->exit_status = 1;
			return ;
		}
		if (chdir(current->cmdv[1]) == -1 && ft_strcmp(current->cmdv[1], "~") != 0)
			exit(1);
	}
	if ( !current->cmdv[1] || ft_strcmp(current->cmdv[1], "../") == 0)
		var_pwd = ft_strjoin("PWD=", get_prev_dir(current->info->env));
	else if (ft_strcmp(current->cmdv[1], "~") == 0)
		var_pwd = ft_strjoin("PWD=", get_home(current->info->env));
	else
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
