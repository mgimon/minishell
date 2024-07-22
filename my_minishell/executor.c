/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/22 20:21:43 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token_list(t_general *info)
{
	int	i;

	i = 0;
	while (i < info->number_of_tokens)
	{
		printf("%s ", info->tokens_list[i].str);
		i++;
	}
	printf("\n");
}

void	tokens_executor(t_general *info)
{
	int		i;
	//char	**sections;

	i = 0;
	print_token_list(info);
	// Si hay pipe
		//split en secciones
		//cuenta cuantas pipes hay y las crea
		//forkea tantos hijos como secciones haya, y envia a cada hijo una seccion y un side de su pipe
			
			//en el subproceso:
			//si no hay builtins, ejecuta cmds en los extremos de las pipes
			//si hay builtins... (implementar)

	// Si no hay pipe
}
