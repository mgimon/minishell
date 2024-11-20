/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:39:19 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/14 21:05:01 by mgimon-c         ###   ########.fr       */
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
	remove_var_from_exports(current, var_equal);
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], var_equal, length) == 0)
		{
			new_env = remove_env_line(current, i);
			if (new_env)
				set_new_env_2(current, new_env, var_equal);
			break ;
		}
		i++;
	}
	if (var_equal)
		free(var_equal);
}

void	if_remove_var_from_env(char ***env, char *var)
{
	char	**new_env;
	char	*var_name;
	size_t	env_len;
	size_t	i;
	size_t	j;

	var_name = NULL;
	if (!check_all_env(env, var, &var_name, &env_len))
		return ;
	new_env = (char **)malloc(sizeof(char *) * (env_len));
	i = 0;
	j = 0;
	while (i < env_len)
	{
		if (!compare_var_name((*env)[i], var_name))
			new_env[j++] = (*env)[i];
		else
			free((*env)[i]);
		i++;
	}
	new_env[j] = NULL;
	set_new_env(new_env, j, var_name, env);
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
		clean_repeateds(&(current->info->exports));
		print_matrix(current->info->exports);
		return ;
	}
	while (current->cmdv[n])
	{
		if (current->cmdv[n][0] >= '0' && current->cmdv[n][0] <= '9')
			exportnovar(current, n);
		else
			add_export_var(current, &new_paths, n);
		n++;
	}
	if (new_paths != NULL)
		matrix_free(new_paths);
}

char	**new_wd_environment(char **oldenv, const char *var_newpwd,
		const char *var_newoldpwd)
{
	int		i;
	int		env_size;
	char	**newenv;

	env_size = 0;
	i = 0;
	while (oldenv[env_size] != NULL)
		env_size++;
	newenv = malloc((env_size + 1) * sizeof(char *));
	if (newenv == NULL)
		return (NULL);
	while (i < env_size)
	{
		if (ft_strncmp(oldenv[i], "PWD=", 4) == 0)
			newenv[i] = ft_strdup(var_newpwd);
		else if (ft_strncmp(oldenv[i], "OLDPWD=", 7) == 0)
			newenv[i] = ft_strdup(var_newoldpwd);
		else
			newenv[i] = ft_strdup(oldenv[i]);
		if (!check_new_env(newenv, i))
			return (NULL);
		i++;
	}
	newenv[env_size] = NULL;
	return (newenv);
}

// only absolute or relative routes
void	execute_cd(t_section *current)
{
	char	*var_pwd;
	char	*var_oldpwd;

	var_pwd = NULL;
	var_oldpwd = NULL;
	check_cd_arguments(current);
	if (current->next || count_lines(current->cmdv) > 2)
		return ;
	var_pwd = get_var_pwd(current);
	var_oldpwd = get_var_oldpwd(current);
	if (try_chdir(current))
	{
		update_pwds(current, &var_pwd, &var_oldpwd);
		update_and_free_env(current, var_pwd, var_oldpwd);
		print_new_pwd(current);
		if_clean_complex_route(current);
		if_backdirs_only(current);
	}
	else
		cdnofile(current);
	free_pwd_vars(var_pwd, var_oldpwd);
}
