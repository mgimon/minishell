/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_25.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:09:25 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/09 21:33:45 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_single_part(const char *part, char **path_parts, int count)
{
	const char	*start;
	int			length;

	start = part;
	while (*part && *part != '/')
		part++;
	length = part - start;
	if (length > 0)
	{
		if (ft_strncmp(start, "..", 2) == 0)
			count = handle_double_dot(path_parts, count);
		else if (ft_strncmp(start, ".", 1) != 0)
		{
			count = copy_path_part(start, length, path_parts, count);
			if (count == -1)
				return (-1);
		}
	}
	return (count);
}

void	cdnofile(t_section *current)
{
	put_str_fd(2, "cd: no such file or directory: ");
	put_str_fd(2, current->cmdv[1]);
	put_str_fd(2, "\n");
	current->info->exit_status = 1;
}

void	exportnovar(t_section *current, int n)
{
	put_str_fd(2, "export: ");
	put_str_fd(2, current->cmdv[n]);
	put_str_fd(2, ": not a valid identifier");
	put_str_fd(2, "\n");
	current->info->exit_status = 1;
}

void	prepare_child_process(t_section *current, int prev_fd, int *pipefd)
{
	if (current->cmdv[0])
	{
		if (ft_strcmp(current->cmdv[0], "grep") == 0 && current->nofile == 1)
			exit(0);
	}
	handle_child_process(current, prev_fd, pipefd);
	setup_signals(0);
}
