/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_23.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:21:30 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/20 21:47:17 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psnorminetter(const char **start, int *count, char ***path_parts,
		int *length)
{
	if (ft_strncmp(*start, "..", 2) == 0)
		*count = handle_double_dot(*path_parts, *count);
	else if (ft_strncmp(*start, ".", 1) != 0)
	{
		*count = copy_path_part(*start, *length, *path_parts, *count);
		if (*count == -1)
			return (-1);
	}
	return (0);
}

void	handle_forks(t_section **current, int *prev_fd, int (*pipefd)[2])
{
	if ((*current)->pid == 0)
	{
		if ((*current)->cmdv[0])
		{
			if ((ft_strcmp((*current)->cmdv[0], "grep") == 0
					|| ft_strcmp((*current)->cmdv[0], "cat") == 0)
				&& (*current)->nofile == 1)
				exit(0);
		}
		handle_child_process(*current, *prev_fd, *pipefd);
		setup_signals(0);
	}
	else
		parent_process(&(*current), &(*prev_fd), *pipefd);
}

void	exexport_printerr(t_section *current, int n)
{
	put_str_fd(2, "export: ");
	put_str_fd(2, current->cmdv[n]);
	put_str_fd(2, ": not a valid identifier\n");
	current->info->exit_status = 1;
}
