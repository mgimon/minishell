/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 21:35:56 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/13 18:45:10 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_str_to_matrix(char ***matrix, char *str)
{
	int		i;
	char	*duplicate;
	char	**new_matrix;

	if (!matrix || !str)
		return ;
	i = 0;
	while ((*matrix) && (*matrix)[i])
		i++;
	new_matrix = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_matrix)
		return ;
	i = 0;
	while ((*matrix) && (*matrix)[i])
	{
		new_matrix[i] = ft_strdup((*matrix)[i]);
		i++;
	}
	duplicate = ft_strdup(str);
	new_matrix[i] = duplicate;
	new_matrix[i + 1] = NULL;
	if (*matrix)
		matrix_free(*matrix);
	*matrix = new_matrix;
}

void	allocate_heredocs(t_section *section, t_token *first, int s)
{
	t_token		*tmp;
	t_token		*section_start;
	int			count;

	tmp = first;
	section_start = NULL;
	if (!first)
		return ;
	skip_to_section(&tmp, s);
	if (!tmp)
	{
		section->heredocs = NULL;
		return ;
	}
	count = count_heredocs(tmp, &section_start);
	initialize_heredocs(section, count);
	if (count > 0)
		process_heredocs(section_start, section->heredocs, count);
}

char	*clean_str_exit(char *str)
{
	int		i;
	int		j;
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
	clean_str_exit_helper(result, str);
	return (result);
}

void	write_in_heredocs(t_section *current, t_general *info)
{
	char		buffer[1024];
	t_heredoc	*tmp_hdoc;

	tmp_hdoc = current->heredocs;
	if (!current || !current->heredocs)
		return ;
	while (tmp_hdoc)
	{
		write_in_heredocs_helper(tmp_hdoc, buffer, info);
		close(tmp_hdoc->fds[1]);
		if (buffer[0] == '\0')
			break ;
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
