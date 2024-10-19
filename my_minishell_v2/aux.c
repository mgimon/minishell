/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:36:53 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 19:41:00 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_general(t_general *info, char **env)
{
	info->number_of_tokens = 0;
	info->tokens_list = NULL;
	info->env = env;
	return (0);
}

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
}

void	init_history(const char *history_file)
{
	using_history();
	if (access(history_file, F_OK) == 0)
		read_history(history_file);
}

void	cleanup(const char *history_file)
{
	write_history(history_file);
}
