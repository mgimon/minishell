/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:58:22 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/31 17:38:02 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_files_per_section(t_token *first)
{
	int		result;
	t_token	*tmp;

	result = 0;
	tmp = first;
	while (tmp && tmp->type != 7)
	{
		if (tmp->type == 6)
			result++;
		tmp = tmp->next;
	}
	return (result);
}

// no bonus, only one command vector per section
// meaning no & or ; etc
int	count_cmdvs_per_section(t_token *first)
{
    int     result;
    t_token *tmp;

    result = 0;
    tmp = first;
    while (tmp && tmp->type != 7)
    {
        if (tmp->type == 1)
		{
            result++;
			tmp = tmp->next;
			while (tmp && tmp->type == 2)
			{
				result++;
				tmp = tmp->next;
			}
		}
		else
        	tmp = tmp->next;
    }
    return (result);
}

t_token	*get_first_in_section(t_token *first, int s)
{
	t_token *tmp;
    int     j;

    tmp = first;
    j = 0;
    while (tmp && j < s)
    {
        if (tmp->type == 7)
            j++;
        tmp = tmp->next;
    }
	return (tmp);
}

void	add_file_to_files(t_token *section_first, t_file *files, int *i, int n)
{
	if (!section_first)
		return ;
	files[*i].string = ft_strdup((section_first->next)->str);
	files[*i].open_mode = section_first->type;
	if (*i == (n - 1))
		files[*i].next = NULL;
	else
		files[*i].next = &files[*i + 1];
	(*i)++;
}
// puede que se pierda la referencia de los fd sobreescritos,
// necesaria para cerrarlos - arreglar
void	open_files_section(t_section *section)
{
	t_file	*tmp;

	tmp = section->files;
	while (tmp)
	{
		if (tmp->open_mode == 3)
		{
			if (section->fd_write != -1)
				close(section->fd_write);
			section->fd_write = open(tmp->string, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		}
		else if (tmp->open_mode == 4)
		{
			if (section->fd_write != -1)
				close(section->fd_write);
			section->fd_write = open(tmp->string, O_WRONLY | O_CREAT | O_APPEND, 0666);
		}
		else if (tmp->open_mode == 5)
		{
			if (section->fd_read != -1)
				close(section->fd_read);
			section->fd_read = open(tmp->string, O_RDONLY);
		}
		tmp = tmp->next;
	}	
}