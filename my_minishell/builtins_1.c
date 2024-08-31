/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:05:39 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/08/29 20:55:53 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_echo(t_section *current)
{
    int i;
    int w;
    int no_newline;

    i = 0;
    no_newline = 1;
    if (!current->cmdv[1])
        return (write(STDOUT_FILENO, "\n", 1), 0);
    if (ft_strcmp(current->cmdv[1], "-n") == 0 && (!current->cmdv[2] || current->cmdv[2][0] == '\0'))
        return (0);
    if (ft_strcmp(current->cmdv[1], "-n") == 0)
    {
        no_newline = 0;
        i = 2;
    }
    else
        i = 1;
    while (current->cmdv[i])
    {
        w = 0;
        while (current->cmdv[i][w])
        {
            write(STDOUT_FILENO, &current->cmdv[i][w], 1);
            w++;
        }
        if (current->cmdv[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (no_newline)
		write(STDOUT_FILENO, "\n", 1);
    return (0);
}


int	execute_pwd(t_section *current)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (current->info->env[i] != NULL)
	{
		if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
		{
			tmp = current->info->env[i];
			break ;
		}
		i++;
	}
	if (!tmp)
		return (0);
	i = 4;
	while (tmp[i])
	{
		write(STDOUT_FILENO, &tmp[i], 1);
		i++;
	}
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	execute_env(t_section *current)
{
	int		i;
	int		j;

	i = 0;
	while (current->info->env[i] != NULL)
	{
		j = 0;
		while (current->info->env[i][j] != '\0')
		{
			write(STDOUT_FILENO, &current->info->env[i][j], 1);
			j++;
		}
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}

void	execute_exit(t_section *current)
{
	char		*str;
	int		status;
	long int	value;

	str = NULL;
	value = 0;
	if (current->next)
		return ;
	if (!current->cmdv[1])
	{
		kill(0, SIGTERM);
		exit(0);
	}
	if (current->cmdv[2])
	{
		put_str_fd(2, "Exit: too many arguments\n");
		exit(1);
	}
	str = clean_str_exit(current->cmdv[1]);
	if (str)
		value = strtol(str, NULL, 10);
	if (!str || errno == ERANGE || value > LONG_MAX || value < LONG_MIN)
	{
		put_str_fd(2, "Exit: numeric argument required\n");
		if (str)
			free(str);
		exit(2);
	}
	status = (int)(value % 256);
	free(str);
	exit(status);
}
