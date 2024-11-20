/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_23.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:58:22 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/20 20:29:40 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_and_write_line(char *buffer, t_heredoc *tmp_hdoc,
		t_general *info)
{
	char	*expanded;
	int		len;

	expanded = expand_var(buffer, info);
	if (expanded)
	{
		len = ft_strlen(expanded) + 1;
		if (write(tmp_hdoc->fds[1], expanded, len - 1) == -1)
		{
			free(expanded);
			return ;
		}
		write(tmp_hdoc->fds[1], "\n", 1);
		free(expanded);
	}
}

void	update_exports(t_section *current, int n)
{
	int		total_exports;
	char	**new_exports;

	total_exports = count_exports(current->info->exports);
	new_exports = copy_exports(current->info->exports, n, total_exports);
	matrix_free(current->info->exports);
	current->info->exports = new_exports;
}

int	find_pwd_index(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp_pipex(env[i], "PWD=", 4) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	**split_current_pwd(char *pwd_line)
{
	if (strlen(pwd_line) == 0)
		return (NULL);
	return (ft_split(pwd_line, '/'));
}

int	calculate_new_len(char **split_pwd, int dirs_back)
{
	int	split_len;
	int	i;

	split_len = 0;
	i = 0;
	while (split_pwd[i] != NULL)
	{
		split_len++;
		i++;
	}
	if (dirs_back >= split_len)
		return (0);
	return (split_len - dirs_back);
}
