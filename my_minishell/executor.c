/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/24 20:25:19 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokens_executor(t_general *info)
{
    // Si hay pipe
        //split en secciones
        //cuenta cuantas pipes hay y las crea
        //forkea tantos hijos como secciones haya, y envia a cada hijo una seccion y un side de su pipe

            //en el subproceso:
            //si no hay builtins, ejecuta cmds en los extremos de las pipes
            //si hay builtins... (implementar)

    // Si no hay pipe

	int		i;
	int		section_end;
	int		sides[2];
	char	**section;
	t_token	*new_tokens_list;

	i = 0;
	section_end = -1;
	if (!info->tokens_list)
		return ;
	new_tokens_list = info->tokens_list;
	
		//section = get_gross_section(new_tokens_list, &section_end);
/*	if (!thereis_pipe(new_tokens_list))
	{
		process_nopipe(section);
		return ;
	}*/
}
