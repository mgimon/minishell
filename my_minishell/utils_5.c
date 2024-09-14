/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 21:35:56 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/14 21:25:02 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_str_to_matrix(char ***matrix, char *str)
{
    int     i;
    char    **new_matrix;

    if (!matrix || !str)
        return;

    i = 0;
    while ((*matrix)[i])
        i++;
    new_matrix = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_matrix)
        return;

    i = 0;
    while ((*matrix)[i])
    {
        new_matrix[i] = (*matrix)[i];
        i++;
    }
    new_matrix[i] = str;
    new_matrix[i + 1] = NULL;

    free(*matrix);
    *matrix = new_matrix;
}

void	allocate_heredocs(t_section *section, t_token *first)
{
    t_token     *tmp;
    t_heredoc   *tmp_hdocs;
    t_heredoc   *new_hdoc;
    int         count;
    
    tmp = first;
    count = 0;
    if (!first)
        return ;
    while (tmp)
    {
        if (ft_strcmp(tmp->str, "<<") == 0)
            count++;
        tmp = tmp->next;
    }
    
    if (!count)
    {
        section->heredocs = NULL;
        return ;
    }

    section->heredocs = malloc(sizeof(t_heredoc));
    tmp_hdocs = section->heredocs;
    tmp = first;
    while (tmp)
    {
        if (ft_strcmp(tmp->str, "<<") == 0)
        {
            if (!tmp->next)
                break ;
            tmp_hdocs->delimiter = ft_strdup((tmp->next)->str);
			pipe(tmp_hdocs->fds);
            if (count > 1)
            {
                new_hdoc = malloc(sizeof(t_heredoc)); 
                tmp_hdocs->next = new_hdoc;
                tmp_hdocs = new_hdoc;
            }
            else
                tmp_hdocs->next = NULL;
            count--;
        }
        tmp = tmp->next;
    }
}

char	*clean_str_exit(char *str)
{
	int	i;
	int	j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i] == ' ')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (NULL);
	while (str[i] >= '0' && str[i] <= '9')
	{
		i++;
		j++;
	}
	while (str[i] == ' ')
		i++;
	if (str[i] != '\0')
		return (NULL);
	result = malloc(sizeof(char) * (j + 1));
	j = 0;
	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result[j] = str[i];
		i++;
		j++;
	}
	result[j] = '\0';
	return (result);
}

void write_in_heredocs(t_section *current)
{
    char		buffer[1024];
    int			len;
	t_heredoc	*tmp_hdoc;

	tmp_hdoc = current->heredocs;
    if (!current || !current->heredocs)
        return ;
    while (tmp_hdoc)
    {
        while (1)
        {
			write(1, "> ", 2);
            len = read(0, buffer, sizeof(buffer) - 1);
            if (len <= 0)
                break;
            buffer[len] = '\0';
            if (len > 0 && buffer[len - 1] == '\n')
                buffer[len - 1] = '\0';
            if (ft_strcmp(buffer, tmp_hdoc->delimiter) == 0)
                break;
			printf("Escribiendo en el pipe heredoc: [%s]\n", buffer);
            if (write(tmp_hdoc->fds[1], buffer, len - 1) == -1)
                return;
			write(tmp_hdoc->fds[1], "\n", 1);
			printf("Escribió salto de línea en pipe heredoc\n");
        }
		close(tmp_hdoc->fds[1]);
        if (len <= 0)
            break;
		tmp_hdoc = tmp_hdoc->next;
    }
}

void	close_section_hdocs_parent(t_section *current)
{
	t_heredoc	*tmp;

	tmp = current->heredocs;
	while (tmp)
	{
		close(tmp->fds[0]);
		close(tmp->fds[1]);
		tmp = tmp->next;
	}
}
