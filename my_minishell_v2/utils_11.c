/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 20:35:51 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/17 21:44:41 by mgimon-c         ###   ########.fr       */
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

void	change_pwd_helper(t_section *current)
{
	if (current->cmdv[1])
	{
		if (!is_directory(current->cmdv[1])
			&& ft_strcmp(current->cmdv[1], "~") != 0)
		{
			put_str_fd(2, "cd: no such file or directory: ");
			put_str_fd(2, current->cmdv[1]);
			write(2, "\n", 1);
			current->info->exit_status = 1;
			return ;
		}
		if (chdir(current->cmdv[1]) == -1
			&& ft_strcmp(current->cmdv[1], "~") != 0)
			exit(1);
	}
}

void	change_pwd(t_section *current, char **content, char **var_pwd)
{
	change_pwd_helper(current);
	if (!current->cmdv[1] || ft_strcmp(current->cmdv[1], "../") == 0)
	{
		*content = get_prev_dir(current->info->env);
		*var_pwd = ft_strjoin("PWD=", *content);
	}
	else if (ft_strcmp(current->cmdv[1], "~") == 0)
	{
		*content = get_home(current->info->env);
		*var_pwd = ft_strjoin("PWD=", *content);
	}
	else
		*var_pwd = ft_strjoin("PWD=", current->cmdv[1]);
}

void	update_oldpwd(t_section *current, char **var_oldpwd, int *i)
{
	while (current->info->env[*i])
	{
		if (ft_strncmp_pipex(current->info->env[*i], "PWD=", 4) == 0)
			*var_oldpwd = ft_strjoin("OLD", current->info->env[*i]);
		(*i)++;
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
