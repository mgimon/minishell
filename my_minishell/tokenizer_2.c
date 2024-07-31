/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 17:26:31 by albealva          #+#    #+#             */
/*   Updated: 2024/07/29 18:20:50 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int recalculate_tokens(t_general *info) 
{
    t_token *current = info->tokens_list;
    int count = 0;
    while (current != NULL) 
    {
        count++;
        current = current->next;
    }

    return (count);
}

void add_token(t_general *info, char *word, int type)
{
    // Faltaría implementar medidas de manejo de errores, pero esa implementación se omite para simplificar.
    t_token *new_token = malloc(sizeof(t_token));
	if (!new_token) return; // Manejo del error simplificado.
    new_token->str = strdup(word); // Copia la palabra en el miembro 'str' del token
    new_token->type = type; // Establece el tipo del token calculado

	// Parte analogía 'enqueue' o puesta en cola, pero para tu estructura
    new_token->next = NULL;
	if (info->tokens_list == NULL) 
	{ // Comprobación si es el primer elemento
        	new_token->prev = NULL;
			info->tokens_list = new_token;
	} 
	else 
	{ // Si ya hay elementos, inserta al final
        	t_token *temp_token = info->tokens_list;
		while (temp_token->next != NULL)
            		temp_token = temp_token->next;
        	temp_token->next = new_token;
        	new_token->prev = temp_token;
    	}
    // Para la demo, imprimir el token y su tipo
    printf("Token: '%s', Type: %d\n", word, type);
}

// Esta función se supone que analiza y determina los tipos de las tokens proporcionadas
void tokenize_input(t_general *info, char *input)
{
    const char delimiters[] = " \t\n"; // Definimos espacios, tabuladores y new lines como delimitadores
    char *token = strtok(input, delimiters);

    while (token != NULL)
    {
        // Lógica simplificada de determinación de typo (sample, para expandir)
        int type = EMPTY; // Un valor por default
        if (strcmp(token, ">") == 0)
	{ // Ejemplo de direccionamiento
            type = TRUNC;
        } 
	else if (strcmp(token, ">>") == 0)
	{
            type = APPEND;
        }
	else if (strcmp(token, "<") == 0)
	{
            type = INPUT;
	}
       	else if (strcmp(token, "|") == 0)
	{
            type = PIPE;
        }
	else if (info->tokens_list == NULL || info->tokens_list->type == PIPE)
	{
            // Enumera cualquier cosa antes de otra cosa como un CMD, instanceof as demostration
			type = CMD;
	}
	else
	{
	// Argumentos u otros tipos siguiendo un comando directamente
		type = ARG;
	}
        // Crear y enlazar este nuevo token al final de nuestra lista / estructura 'info'
        add_token(info, token, type);
        // Obtener el próximo token (get next token)
        token = strtok(NULL, delimiters);
    }
}

void free_tokens_list(t_general *info)
{
    t_token *current = info->tokens_list;
    t_token *next;
    while (current != NULL)
    {
        next = current->next;
	// Liberar la memoria asignada para la cadena del token si existe
        if (current->str != NULL)
	{
            free(current->str);
            current->str = NULL; // Se recomienda establecer punteros liberados a NULL para evitar usar memoria ya liberada.
        }
        // Liberar la estructura t_token en sí
        free(current);
        current = next;
    }

    // Asegúrate de restablecer el puntero de la lista en la estructura general a NULL
    info->tokens_list = NULL;
}

/* Asegúrate de compilar y vincular este código adecuadamente,
   incluir headers parents para 't_general' y 't_token', además de otras librerías usadas. */
