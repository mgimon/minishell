/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:26:33 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/14 21:04:50 by mgimon-c         ###   ########.fr       */
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

void	iterate_str(const char **str, int *sign, long *result)
{
	*sign = 1;
	*result = 0;
	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (**str == '-')
	{
		*sign = -1;
		(*str)++;
	}
	else if (**str == '+')
		(*str)++;
}

int	ft_atol_overflow(const char *str, long *result)
{
	int	sign;
	int	digit;

	iterate_str(&str, &sign, result);
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (sign == 1 && (*result > (LONG_MAX - digit) / 10))
		{
			*result = LONG_MAX;
			return (-1);
		}
		else if (sign == -1 && (*result > (LONG_MIN + digit) / -10))
		{
			*result = LONG_MIN;
			return (-1);
		}
		*result = *result * 10 + digit;
		str++;
	}
	if (*str != '\0')
		return (-2);
	*result *= sign;
	return (0);
}

void	check_argc_exit(t_section *current)
{
	if (current->cmdv[2])
	{
		put_str_fd(2, "Exit: too many arguments\n");
		exit(1);
	}
}
