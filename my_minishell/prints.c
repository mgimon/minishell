/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:18:36 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/31 15:32:52 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_token_list(t_general *info)
{
    int i;

    i = 0;
    printf("Print token list: \n");
    while (i < info->number_of_tokens)
    {
        printf("%s ", info->tokens_list[i].str);
        i++;
    }
    printf("\n");
}

void    print_matrix(char **matrix)
{
	int i;

    i = 0;
    if (!matrix)
        return ;
    while (matrix[i])
    {
        printf("%s\n", matrix[i]);
        i++;
    }
	printf("\n");
}

void	print_sections_info(t_section *section)
{
	t_section	*tmp;
	t_file		*file;
	int			j;
	int			i;

	tmp = section;
	j = 0;
    while (tmp)
	{
		file = tmp->files;
		i = 0;
		printf("Section %d info:\n", j);
		printf("Cmdv is:\n");
		print_matrix(tmp->cmdv);
		printf("paths are:\n");
		print_matrix(tmp->paths);
		printf("\nthe right path is:\n");
		printf("\n%s\n\n", tmp->path);
		while (file)
		{
			printf("El file %d se llama %s\n", i, file->string);
			printf("El file %d tiene open mode %d\n", i, file->open_mode);
			printf("El fd_read es %d\n", tmp->fd_read);
			printf("El fd_write es %d\n", tmp->fd_write);
			printf("\n");
			printf("\n");
			file = file->next;
			i++;
		}
		j++;
		tmp = tmp->next;
	}
}
