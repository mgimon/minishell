/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:37:51 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/24 20:22:59 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.c"

int	count_sections(t_token *first)
{
	t_token	*tmp;
	int	i;

	tmp = first;
	if (!tmp)
		return (0);
	i = 1;
	while (tmp)
	{
		tmp = tmp->next;
		if (tmp->type == 7)
			i++;
	}
	return (i);
}
//if end of section is end of sentence, section_end is set to 0
//if it finds a pipe, flag is set to the index of the pipe
char    **get_gross_section(t_token *first, int *section_end)
{
    int     i;
    char    **gross_section;
    t_token *tmp;

    i = 0;
    tmp = first;
    if (!first)
        return (NULL);
    while (tmp && tmp->type < 7)
    {
        tmp = tmp->next;
        i++;
    }
    if (!tmp)
        *section_end = 0;
    else if (tmp->type == 7)
        *section_end = i;
    gross_section = malloc(sizeof(char *) * i);
    if (gross_section == NULL)
        return (NULL);
    tmp = first;
    i = 0;
    while (tmp && tmp->type < 7)
    {
        gross_section[i] = tmp->str;
        tmp = tmp->next;
        i++;
    }
    gross_section[i] = NULL;
    return (gross_section);
}
