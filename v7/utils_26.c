/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_26.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:09:25 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/09 21:33:45 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_cd_arguments(t_section *current)
{
	if (count_lines(current->cmdv) > 2)
	{
		put_str_fd(2, "cd: too many arguments\n");
		current->info->exit_status = 1;
	}
}

int	try_chdir(t_section *current)
{
	if ((chdir(current->cmdv[1]) != -1) || (count_lines(current->cmdv) == 1)
		|| (ft_strcmp(current->cmdv[1], "~") == 0))
		return (1);
	return (0);
}

void	update_and_free_env(t_section *current, char *var_pwd, char *var_oldpwd)
{
	char	**new_env;
	int		i;

	new_env = new_wd_environment(current->info->env, var_pwd, var_oldpwd);
	matrix_free(current->info->env);
	free(var_pwd);
	if (var_oldpwd)
		free(var_oldpwd);
	current->info->env = new_env;
	i = 0;
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
		{
			put_str_fd(2, "El viejo pwd es: ");
			put_str_fd(2, current->info->env[i]);
			put_str_fd(2, "\n");
		}
		i++;
	}
}

void	print_new_pwd(t_section *current)
{
	int	i;

	i = 0;
	while (current->info->env[i])
	{
		if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
		{
			put_str_fd(2, "El nuevo pwd es: ");
			put_str_fd(2, current->info->env[i]);
			put_str_fd(2, "\n");
		}
		i++;
	}
}

void	free_pwd_vars(char *var_pwd, char *var_oldpwd)
{
	if (var_pwd)
		free(var_pwd);
	if (var_oldpwd)
		free(var_oldpwd);
}
