/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:56:14 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/18 16:31:45 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenizer(char *input)
{
	t_token	*list;
	int		n;

	(void)input;
	n = 42;
	list = malloc(sizeof(t_token) * n);
	//rellenar la lista de tokens
	return (list);
}
