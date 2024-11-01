/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:26:33 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/10/29 20:45:10 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_var(const char *input, t_general *info)
{
    char	*result = NULL;
    int		len = strlen(input);
    char	temp[1024] = {0};  // Buffer temporal para construir el resultado expandido
    char	var_name[256] = {0};
    int		temp_index = 0;  // Índice para el buffer temporal
    int		var_index = 0;

    for (int i = 0; i < len; i++) {
        // Expandir el estado de salida "$?"
        if (input[i] == '$' && input[i + 1] == '?') {
            i += 1;  // Avanza para saltarse el '?'
            // Expande "$?"
            char *exit_status_str = expand_dollar_question(info);
            if (exit_status_str != NULL) {
                strcpy(temp + temp_index, exit_status_str);
                temp_index += strlen(exit_status_str);
                free(exit_status_str);
            }
            continue;  // Continúa al siguiente ciclo para no procesar el '?'
        }

        // Expandir otras variables
        if (input[i] == '$') {
            i++;  // Avanza para saltarse el '$'
            var_index = 0;
            // Captura el nombre de la variable de entorno
            while (i < len && !is_special_separator(input[i]) && (isalnum(input[i]) || input[i] == '_')) {
                var_name[var_index++] = input[i++];
            }
            i--;  // Retrocede un carácter porque el bucle ha avanzado uno de más.
            var_name[var_index] = '\0';

            // Obtiene el valor de la variable de entorno
            char *value = get_env_var(info, var_name);
            if (value) {
                strcpy(temp + temp_index, value);
                temp_index += strlen(value);
            }
        } else {
            // Copia el contenido literal al buffer temporal
            temp[temp_index++] = input[i];
        }
    }

    // Duplica el resultado expandido
    result = strdup(temp);
    return result;
}

void	write_in_heredocs_helper(t_heredoc *tmp_hdoc, char *buffer, t_general *info)
{
	int		len;
	char	*expanded;

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
		expanded = expand_var(buffer, info);
		len = ft_strlen(expanded) + 1;
		if (write(tmp_hdoc->fds[1], expanded, len - 1) == -1)
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
