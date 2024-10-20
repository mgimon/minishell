/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:50:14 by albealva          #+#    #+#             */
/*   Updated: 2024/10/16 21:16:14 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(t_general *info, char *input, int *print_mode)
{
	handle_input(info, input, print_mode);
	info->sections = create_sections_list(info);
	executor(info);
	free(input);
	free_sections_list(info->sections);
	free_tokens_list(info->tokens_list);
}

void	initialize_environment(t_general *info, char **env,
		const char *history_file)
{
	init_general(info, env);
	setup_signals();
	init_history(history_file);
	set_paths_and_env(info, env);
}

void	initialize_program_settings(int argc, char **argv,
		const char **history_file, int *print_mode)
{
	(void)argc;
	(void)argv;
	*history_file = ".minishell_history";
	*print_mode = 1;
}
