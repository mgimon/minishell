/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_aux_7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:35:06 by albealva          #+#    #+#             */
/*   Updated: 2024/11/13 19:11:09 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_dollar_question(t_general *info)
{
	char	*result;

	result = (char *)my_malloc(info, 12);
	if (result == NULL)
		return (NULL);
	result = trolft_itoa(info->exit_status, info);
	return (result);
}

char	mark_char(char c)
{
	return (c | 0x80);
}

char	unmark_char(char c)
{
	return (c & 0x7F);
}

int	is_marked(char c)
{
	return ((c & 0x80) != 0);
}
