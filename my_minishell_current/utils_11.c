/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 20:35:51 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/25 21:29:14 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_export_var(t_section *current, char **new_paths, int n)
{
	add_str_to_matrix(&current->info->exports,
		ft_strjoin("declare -x ", current->cmdv[n]));
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

//*prev_dir[1] = '\0';
char	*get_pdir_helper(size_t len, char **pwd, char **prev_dir)
{
	size_t	i;
	size_t	k;

	i = len;
	while (i > 0)
	{
		if (*pwd[i - 1] == '/')
			break ;
		i--;
	}
	if (i == 0)
	{
		*prev_dir = (char *)malloc(2 * sizeof(char));
		*prev_dir[0] = '.';
		return (*prev_dir[1] = '\0', *prev_dir);
	}
	*prev_dir = (char *)malloc((i + 1) * sizeof(char));
	k = 0;
	while (k < i)
	{
		*prev_dir[k] = *pwd[k];
		k++;
	}
	*prev_dir[i] = '\0';
	return (*prev_dir);
}

int	has_slash(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	i--;
	i--;
	while (i > 0)
	{
		if (str[i] == '/')
			return (1);
		i--;
	}
	return (0);
}

void	remove_endslash(char **var)
{
	int		i;
	int		n;
	char	*newvar;

	i = 0;
	n = 0;
	while ((*var)[i])
		i++;
	i--;
	if ((*var)[i] != '/')
		return ;
	newvar = malloc(sizeof(char) * (i + 1));
	while (n < i)
	{
		newvar[n] = (*var)[n];
		n++;
	}
	newvar[n] = '\0';
	free(*var);
	*var = newvar;
}

int	remove_dots_helper(char *var_pwd, int *i, char c)
{
	(*i)--;
	if (var_pwd[*i] != c)
		return (0);
	return (1);
}

int	remove_dots(char **var_pwd)
{
	int		i;
	int		n;
	char	*new_varpwd;

	i = 0;
	n = 0;
	while ((*var_pwd)[i])
		i++;
	if (i < 7)
		return (0);
	if (!remove_dots_helper((*var_pwd), &i, '.'))
		return (0);
	if (!remove_dots_helper((*var_pwd), &i, '.'))
		return (0);
	if (!remove_dots_helper((*var_pwd), &i, '/'))
		return (0);
	new_varpwd = malloc(sizeof(char) * (i + 1));
	while (n < i)
	{
		new_varpwd[n] = (*var_pwd)[n];
		n++;
	}
	new_varpwd[n] = '\0';
	free(*var_pwd);
	return (*var_pwd = new_varpwd, 1);
}

void	remove_lastdir_helper(char *new_varpwd, char **var_pwd, int *i, int *n)
{
    while ((*n) < (*i))
    {
        new_varpwd[*n] = (*var_pwd)[*n];
        (*n)++;
    }
    if ((*var_pwd)[*i] == '=')
    {
        new_varpwd[*n] = (*var_pwd)[*n];
        (*n)++;
    }
    new_varpwd[*n] = '\0';
}

void	remove_lastdir(t_section *current, char **var_pwd)
{
	int		i;
	int		n;
	char	*new_varpwd;

	i = 0;
	n = 0;
	if (!(*var_pwd) || !(current->cmdv)
		|| count_lines(current->cmdv) == 1)
		return ;
	if (ft_strcmp(current->cmdv[1], "..") != 0
		&& ft_strcmp(current->cmdv[1], "/..") != 0
		&& ft_strcmp(current->cmdv[1], "../") != 0)
		return ;
	if (ft_strcmp((*var_pwd), "PWD=") == 0)
		return ;
	while ((*var_pwd)[i])
		i++;
	while ((*var_pwd)[i] != '/' && (*var_pwd)[i] != '=')
		i--;
	new_varpwd = malloc(sizeof(char) * (i + 1));
	remove_lastdir_helper(new_varpwd, var_pwd, &i, &n);
	free(*var_pwd);
	*var_pwd = new_varpwd;
}

void	change_last_subdir_helper(char **var_pwd, char *result, int *i, int *n)
{
    *i = 0;
    while ((*i) < (*n))
    {
        result[*i] = (*var_pwd)[*i];
        (*i)++;
    }
    result[*i] = '\0';
}

void	change_last_subdir(t_section *current, char **var_pwd)
{
	int		i;
	int		n;
	char	*result;
	char	*tmp;

	if (!current->cmdv || !var_pwd)
		return ;
	i = 0;
	while ((*var_pwd)[i])
		i++;
	n = i;
	while ((*var_pwd)[n] != '/' && n > 0)
		n--;
	result = malloc(sizeof(char) * (n + 1));
	if (!result)
		return ;
	change_last_subdir_helper(var_pwd, result, &i, &n);
	tmp = ft_strjoin(result, (current->cmdv[1] + 2));
	free(result);
	free(*var_pwd);
	*var_pwd = ft_strdup(tmp);
	free(tmp);
}

int	has_dots_and_subdir(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	while (i > 0)
	{
		if (i >= 2 && str[i] == '/')
		{
			if (str[i - 1] == '.')
			{
				if (str[i - 2] == '.')
					return (1);
			}
		}
		i--;
	}
	return (0);
}

void	update_oldpwd(char **var_oldpwd, char **var_pwd)
{
	if (*var_oldpwd)
	{
		free(*var_oldpwd);
		*var_oldpwd = ft_strjoin("OLD", *var_pwd);
	}
}

void	upwds_one(t_section *current, char *tmp, char **var_pwd, char **var_oldpwd)
{
	char	*clean_home;

	clean_home = get_home(current->info->env);
	if ((count_lines(current->cmdv) == 1)
		|| (ft_strcmp(current->cmdv[1], "~") == 0))
		chdir(clean_home);
	update_oldpwd(var_oldpwd, var_pwd);
	tmp = ft_strjoin("PWD=", clean_home);
	free(*var_pwd);
	*var_pwd = ft_strdup(tmp);
	free(tmp);
	free(clean_home);
}
void	upwds_two(char **var_pwd, char **var_oldpwd)
{
	update_oldpwd(var_oldpwd, var_pwd);
	free(*var_pwd);
	*var_pwd = ft_strjoin("PWD=", "/");
}
void	upwds_three(t_section *current, char **var_pwd, char **var_oldpwd)
{
	update_oldpwd(var_oldpwd, var_pwd);
	change_last_subdir(current, var_pwd);
}
void	upwds_four(t_section *current, char *tmp, char **var_pwd, char **var_oldpwd)
{
	update_oldpwd(var_oldpwd, var_pwd);
	tmp = ft_strjoin("PWD=", current->cmdv[1]);
	free(*var_pwd);
	*var_pwd = ft_strdup(tmp);
	free(tmp);
}
void	upwds_five(t_section *current, char *tmp, char **var_pwd, char **var_oldpwd)
{
	update_oldpwd(var_oldpwd, var_pwd);
	free(*var_pwd);
	tmp = ft_strjoin(((*var_oldpwd) + 3), "/");
	*var_pwd = ft_strjoin(tmp, current->cmdv[1]);
	free(tmp);
}

void	update_pwds(t_section *current, char **var_pwd, char **var_oldpwd)
{
	char	*tmp;

	tmp = NULL;
	if ((count_lines(current->cmdv) == 1)
		|| (ft_strcmp(current->cmdv[1], "~") == 0))
		upwds_one(current, tmp, var_pwd, var_oldpwd);
	else if (ft_strcmp(current->cmdv[1], ".") == 0)
		return ;
	else if (ft_strcmp(current->cmdv[1], "/") == 0)
		upwds_two(var_pwd, var_oldpwd);
	else if (has_dots_and_subdir(current->cmdv[1]))
		upwds_three(current, var_pwd, var_oldpwd);
	else if (has_slash(current->cmdv[1]))
		upwds_four(current, tmp, var_pwd, var_oldpwd);
	else
		upwds_five(current, tmp, var_pwd, var_oldpwd);
	remove_endslash(var_pwd);
	remove_endslash(var_oldpwd);
	remove_dots(var_pwd);
	remove_lastdir(current, var_pwd);
}

char	*get_var_pwd(t_section *current)
{
	int	i;

	i = 0;
	while (current->info->env[i])
	{
		if (ft_strncmp(current->info->env[i], "PWD=", 4) == 0)
			return (ft_strdup(current->info->env[i]));
		i++;
	}
	return (ft_strdup("PWD="));
}

char	*get_var_oldpwd(t_section *current)
{
	int	i;

	i = 0;
	while (current->info->env[i])
	{
		if (ft_strncmp(current->info->env[i], "OLDPWD=", 7) == 0)
			return (ft_strdup(current->info->env[i]));
		i++;
	}
	return (ft_strdup("OLDPWD="));
}


// RED 13 + 1var
void	remove_var_from_exports(t_section *current, char *var_equal)
{
	char	**new_exports;
	char	*linedeclare;
	int		i;
	int		j;
	int		k;
	int		n;

	i = 0;
	j = 0;
	k = 0;
	n = 0;
	linedeclare = NULL;
	if (!current || !current->cmdv || !current->info->exports)
		return ;
	if (!current->cmdv[1])
		return ;
	while (current->info->exports[i])
	{
		linedeclare = ft_strjoin("declare -x ", var_equal);
		if (ft_strncmp_pipex(linedeclare, current->info->exports[i], ft_strlen(linedeclare)) == 0)
			n = i;
		i++;
		if (linedeclare)
			free(linedeclare);
	}
	if (!n)
		return ;
	new_exports = malloc(sizeof(char *) * i);
	while (j < i)
	{
		if (j != n)
			new_exports[k++] = ft_strdup(current->info->exports[j]);
		j++;
	}
	new_exports[k] = NULL;
	matrix_free(current->info->exports);
	current->info->exports = new_exports;
}
