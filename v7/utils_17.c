/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_17.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:50:32 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/14 21:00:59 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int		pwd_index;
	char	*pwd_line;
	char	**split_pwd;
	char	*new_pwd;
	char	**new_env;

	if (dirs_back < 1)
		return ;
	pwd_index = find_pwd_index(current->info->env);
	if (pwd_index == -1)
		return ;
	pwd_line = current->info->env[pwd_index] + 4;
	split_pwd = split_current_pwd(pwd_line);
	if (split_pwd == NULL)
		return ;
	new_pwd = construct_new_pwd(split_pwd, calculate_new_len(split_pwd,
				dirs_back));
	if (new_pwd == NULL)
		return ;
	new_env = replace_pwd_in_env(current->info->env,
			env_size(current->info->env), pwd_index, new_pwd);
	if (new_env == NULL)
		return ;
	cleanup_and_replace_env(current, split_pwd, new_env);
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
