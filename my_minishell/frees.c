/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 19:56:25 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/17 21:50:06 by mgimon-c         ###   ########.fr       */
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
		if (files->string)
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
		if (first->paths)
			matrix_free(first->paths);
		free(first->cmdv);
		if (first->gottofree)
			free_files(first->files);
		free(first->path);
		first = tmp;
	}
	free(first_ptr);
}

void	free_tokens_list(t_token *first)
{
    t_token *current;
    t_token *next_node;

	current = first;
	next_node = NULL;
    while (current)
    {
        next_node = current->next;
        if (current->str)
            free(current->str);
        free(current);
        current = next_node;
    }
	first = NULL;
}

void	free_info(t_general *info)
{
	if (info->env)
		matrix_free(info->env);
	if (info->exports)
		matrix_free(info->exports);
	if (info->paths)
		matrix_free(info->paths);
}
