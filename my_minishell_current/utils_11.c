/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 20:35:51 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/20 21:16:46 by mgimon-c         ###   ########.fr       */
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

void	update_pwds(t_section *current, char **var_pwd, char **var_oldpwd)
{
	char	*clean_varpwd;
	char	*clean_varoldpwd;
	char	*tmp;

	// dereference *vars
	// get clean var_pwd and var_oldpwd;
	clean_varpwd = ft_strdup((*var_pwd) + 4);
	clean_varoldpwd = ft_strdup((*var_oldpwd) + 7);
	// gestionar ../
	// check if count_lines(current->cmdv) == 1 to update var_pwd to home
	// check if current->cmdv[1] == '~' to update var_pwd to home
	if ((count_lines(current->cmdv) == 1) || (ft_strcmp(current->cmdv[1], "~") == 0))
	{
		put_str_fd(2, "heyyyy\n");
		if (count_lines(current->cmdv) == 1)
			chdir(get_home(current->info->env));
		if (*var_oldpwd)
		{
			free(*var_oldpwd);
			*var_oldpwd = ft_strdup(*var_pwd);
		}
		tmp = ft_strjoin("PWD=", get_home(current->info->env));
		free(*var_pwd);
		*var_pwd = ft_strdup(tmp);
		free(tmp);
	}
	// check if current->cmdv[1] is absolute route (has slash) to update var_pwd to current->cmdv[1]
	

	// update tmp_var_pwd to var_oldpwd + slash
	// update var_pwd to tmp_var_pwd + current_cmdv[1]
}

