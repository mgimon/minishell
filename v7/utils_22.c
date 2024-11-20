/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_22.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:58:22 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/20 20:29:40 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_file_open(t_section *section, t_file *tmp, t_heredoc **tmp_hdocs)
{
	if (tmp->open_mode == 3)
		open_truncate(section, tmp);
	else if (tmp->open_mode == 4)
		open_append(section, tmp);
	else if (tmp->open_mode == 5 || tmp->open_mode == 9)
	{
		close_prev_fdread(section);
		if (tmp->open_mode == 9)
		{
			if (*tmp_hdocs)
				section->fd_read = (*tmp_hdocs)->fds[0];
			*tmp_hdocs = (*tmp_hdocs)->next;
		}
		else
		{
			section->fd_read = open(tmp->string, O_RDONLY);
			if (section->fd_read == -1)
			{
				put_str_fd(2, tmp->string);
				put_str_fd(2, ": No such file or directory\n");
				section->nofile = 1;
			}
		}
	}
}

void	skip_to_section(t_token **tmp, int s)
{
	int	section_index;

	section_index = 0;
	while (*tmp && section_index < s)
	{
		if ((*tmp)->type == 7)
			section_index++;
		*tmp = (*tmp)->next;
	}
}

int	count_heredocs(t_token *tmp, t_token **section_start)
{
	int	count;

	count = 0;
	*section_start = tmp;
	while (tmp && tmp->type != 7)
	{
		if (ft_strcmp(tmp->str, "<<") == 0)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

void	initialize_heredocs(t_section *section, int count)
{
	if (count == 0)
		section->heredocs = NULL;
	else
		section->heredocs = malloc(sizeof(t_heredoc));
}

void	process_heredocs(t_token *section_start, t_heredoc *heredocs, int count)
{
	t_heredoc	*tmp_hdocs;
	t_heredoc	*new_hdoc;
	t_token		*tmp;

	tmp_hdocs = heredocs;
	new_hdoc = NULL;
	tmp = section_start;
	allocate_heredocs_helper(tmp, tmp_hdocs, new_hdoc, count);
}
