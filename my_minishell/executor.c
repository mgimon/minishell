/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/23 20:25:05 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token_list(t_general *info)
{
	int	i;

	i = 0;
	printf("Print token list: \n");
	while (i < info->number_of_tokens)
	{
		printf("%s ", info->tokens_list[i].str);
		i++;
	}
	printf("\n");
}

void	print_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	printf("\nPrint matrix: \n");
	while (matrix[i] != NULL)
	{
		printf("%s\n", matrix[i]);
		i++;
	}
}

//if end of section is end of sentence, flag is set to 0
//if it finds a pipe, flag is set to the index of the pipe
char	**get_gross_section(t_token *first, int *flag)
{
	int		i;
	char	**gross_section;
	t_token	*tmp;

	i = 0;
	tmp = first;
	if (!first)
		return (NULL);
	while (tmp && tmp->type < 6)
	{
		tmp = tmp->next;
		i++;
	}
	if (!tmp)
		*flag = 0;
	else if (tmp->type == 6)
		*flag = i;
	gross_section = malloc(sizeof(char *) * i);
	if (gross_section == NULL)
		return (NULL);
	tmp = first;
	i = 0;
	while (tmp && tmp->type < 6)
	{
		gross_section[i] = tmp->str;
		tmp = tmp->next;
		i++;
	}
	gross_section[i] = NULL;
	return (gross_section);
}

int	thereis_pipe(t_token *tokens_list)
{
	t_token	*tmp;

	tmp = tokens_list;
	while (tmp)
	{
		if (tmp->type == 6)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

//cleans command section
//and
//creates subprocesses and pipes dinamically to handle commands
void	process_gross_section(char **section, int *sides, int pipeflag, int firstcall)
{
	(void)sides;
	(void)pipeflag;
	print_matrix(section);
}

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
	int		flag;
	int		pipeflag;
	int		sides[2];
	char	**section;
	t_token	*new_tokens_list;
	pid_t	process;

	i = 0;
	flag = -1;
	if (!info->tokens_list)
		return ;
	new_tokens_list = info->tokens_list;
	section = get_gross_section(new_tokens_list, &flag);
/*	if (!thereis_pipe(new_tokens_list))
	{
		process_gross_section_nopipe(section);
		return ;
	}*/
//	pipe(sides);
	process = fork();
	if (process < 0)
		return ;
	if (process == 0)
	{
		if (!new_tokens_list->next)
			return ;
		process_gross_section(section, sides, 1, 0);
		pipeflag = -1;
		while (new_tokens_list)
		{
			if (flag != -1 && flag != 0)
			{
				while (i < (flag + 1))
				{
					new_tokens_list = new_tokens_list->next;
					i++;
				}
				flag = -1;
				section = get_gross_section(new_tokens_list, &flag);
				// fork
				process_gross_section(section, sides, pipeflag, 1);
				// end fork
				pipeflag *= -1;
			}
			else
				new_tokens_list = new_tokens_list->next;
		}
	}
	else
		wait(NULL); // falta recoger retornos de los hijos
}
