/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:26:33 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/26 20:36:49 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_in_heredocs_helper(t_heredoc *tmp_hdoc, char *buffer)
{
	int	len;

	while (1)
	{
		write(1, "> ", 2);
		len = read(0, buffer, 1023);
		if (len <= 0)
			break ;
		buffer[len] = '\0';
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';
		if (ft_strcmp(buffer, tmp_hdoc->delimiter) == 0)
			break ;
		if (write(tmp_hdoc->fds[1], buffer, len - 1) == -1)
			return ;
		write(tmp_hdoc->fds[1], "\n", 1);
	}
}
