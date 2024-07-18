/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:56:14 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/18 14:39:25 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenizer(char *input)
{
	t_token	*first;
	(void)input;

	// mallocear todos los nodos de la lista enlazada
	first = (t_token *)malloc(sizeof(t_token) * 1);
	first->str = "Hola";
	first->type = 1;
	first->next = NULL;
	first->prev = NULL;	
	return (first);
}
