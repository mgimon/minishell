/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 19:56:25 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/11 20:53:20 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	matrix_free(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_files(t_file *files)
{
	t_file	*tmp;

	while (files)
	{
		tmp = files->next;
		free(files->string);
		free(files);
		files = tmp;
	}	
}

void	free_sections_list(t_section *first)
{
	t_section *first_ptr;
	t_section *tmp;

	if (!first)
		return ;
	first_ptr = first;
	while (first)
	{
		tmp = first->next;
		matrix_free(first->paths);
	/*	matrix_free(first->cmdv);*/
		free_files(first->files);
		free(first->path);
		// unnecessary?
	/*	free(first);*/
		first = tmp;
	}
	free(first_ptr);
}
