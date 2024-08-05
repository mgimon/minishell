/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:37:51 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/05 17:04:16 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_files_section(t_section *section, t_token *first, int s)
{
	t_token	*tmp;
	t_file	*files;
	int		n;
	int		i;

	i = 0;
	tmp = get_first_in_section(first, s);
	n = count_files_per_section(tmp);
	printf("Section found. Contains %d files\n\n", n);
	if (n == 0)
	{
		section->files = NULL;
		return ;
	}
	files = malloc(sizeof(t_file) * n);
	while (tmp && tmp->type != 7)
	{
		if (tmp->next && tmp->type >= 3 && tmp->type <= 5)
			add_file_to_files(tmp, files, &i, n);
		tmp = tmp->next;
	}
	section->files = files;
	open_files_section(section);
}

void	set_cmdv_section(t_section *section, t_token *first, int s)
{
	t_token	*tmp;
	char	**cmdv;
	int		n;
	int		i;

	i = 0;
	tmp = get_first_in_section(first, s);
	n = count_cmdvs_per_section(tmp);
	cmdv = malloc(sizeof(char *) * (n + 1));
	while (tmp && tmp->type != 7)
	{
		if (tmp->type == 1)
		{
			cmdv[i] = tmp->str;
			i++;
			tmp = tmp->next;
			while (tmp && tmp->type == 2)
			{
				cmdv[i] = tmp->str;
				i++;
				tmp = tmp->next;
			}
			break ;
		}
		tmp = tmp->next;
	}
	cmdv[i] = NULL;
	section->cmdv = cmdv;
}

void	init_section_objects(t_general *info, t_token *first, t_section *section, int s)
{
	int	i;

	i = 0;
	set_files_section(section, first, s);
	set_cmdv_section(section, first, s);
	section->env = info->env;
	while (info->paths[i])
		i++;
	section->paths = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (info->paths[i])
	{
		section->paths[i] = ft_strdup(info->paths[i]);
		i++;
	}
	section->paths[i] = NULL;
	set_cmd_in_paths(section);
	set_path(section);
}

void	init_section_all_null(t_section *section)
{
	section->fd_read = -1;
	section->fd_write = -1;
	section->env = NULL;
	section->paths = NULL;
	section->path = NULL;
	section->cmdv = NULL;
}

t_section	*create_sections_list(t_general *info)
{   
    int         i;
    int         n;
    t_token     *first;
    t_section   *sections_list;

	i = 0;
    first = info->tokens_list;
    n = count_sections(first);
    if (!n)
        return (NULL);
    sections_list = malloc(sizeof(t_section) * n);
	while (i < n)
	{
		if (i == (n - 1))
			sections_list[i].next = NULL;
		else
			sections_list[i].next = &sections_list[i + 1];
		init_section_all_null(&sections_list[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		init_section_objects(info, first, &sections_list[i], i);
		i++;
	}
	return (sections_list);
}
