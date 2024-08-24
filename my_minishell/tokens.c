/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:35:42 by albealva          #+#    #+#             */
/*   Updated: 2024/08/23 19:18:02 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_token *create_node(char *str, int type)
{
    t_token *new_node = (t_token *)malloc(sizeof(t_token));
    if (!new_node)
        return NULL;
    
    new_node->str = ft_strdup(str);
    new_node->type = type;
    new_node->prev = NULL;
    new_node->next = NULL;
    
    return new_node;
}

t_token *reverse_copy_list(t_token *tokens_list)
{
    t_token *reversed_list = NULL;
    t_token *current = tokens_list;
    t_token *new_node = NULL;

    while (current)
    {
        new_node = create_node(current->str, current->type);
        if (!new_node)
        {
            while (reversed_list)
            {
                t_token *temp = reversed_list;
                reversed_list = reversed_list->next;
                free(temp->str);
                free(temp);
            }
            return NULL;
        }

        new_node->next = reversed_list;
        if (reversed_list)
            reversed_list->prev = new_node;
        reversed_list = new_node;

        current = current->next;
    }

    return (reversed_list);
}

void process_incomplete_token(char *token, t_general *info) {
    char *rest = token;

    while (*rest) {
        while (*rest == ' ') {
            rest++;
        }
        if (*rest == '\0') {
            break;
        }
        char *token_start = rest;
        while (*rest && *rest != ' ') {
            rest++;
        }
        size_t length = rest - token_start;
        if (length > 0) {
            char *inner_token = malloc(length + 1);
            if (!inner_token) {
                perror("Malloc failed");
                exit(EXIT_FAILURE);
            }
            strncpy(inner_token, token_start, length);
            inner_token[length] = '\0';

            t_token *new_token = malloc(sizeof(t_token));
            if (!new_token) {
                perror("Malloc failed");
                exit(EXIT_FAILURE);
            }

            new_token->str = inner_token;

            new_token->type = ARG;
            new_token->next = info->tokens_list;
            if (info->tokens_list != NULL) {
                info->tokens_list->prev = new_token;
            }
            new_token->prev = NULL;
            info->tokens_list = new_token;

            printf("Token: '%s' | Tipo: %d\n", new_token->str, new_token->type);
            info->number_of_tokens++;
        }
    }
}

void tokenize_input(t_general *info, char *input) {
    t_token *new_token;
    char *rest = input;
    int first_token = 1;
    int previous_redirect = 0;
    char *incomplete_token = NULL;
    char quote = '\0';

    info->number_of_tokens = 0;
    info->tokens_list = NULL;

    while (*rest) {
        while (*rest == ' ') {
            rest++;
        }
        if (*rest == '\0') {
            break;
        }

        if (*rest == '"' || *rest == '\'') {
            quote = *rest;
            rest++;
            char *token_start = rest;
            while (*rest && *rest != quote) {
                rest++;
            }
            if (*rest == quote) {
                size_t length = rest - token_start;
                char *inner_token = malloc(length + 1);
                if (!inner_token) {
                    perror("Malloc failed");
                    exit(EXIT_FAILURE);
                }
                strncpy(inner_token, token_start, length);
                inner_token[length] = '\0';
                rest++;

                new_token = malloc(sizeof(t_token));
                if (!new_token) {
                    perror("Malloc failed");
                    exit(EXIT_FAILURE);
                }

                new_token->str = inner_token;

                if (first_token) {
                    new_token->type = CMD;
                    first_token = 0;
                } else {
                    new_token->type = ARG;
                }

                new_token->next = info->tokens_list;
                if (info->tokens_list != NULL) {
                    info->tokens_list->prev = new_token;
                }
                new_token->prev = NULL;
                info->tokens_list = new_token;

                printf("Token: '%s' | Tipo: %d\n", new_token->str, new_token->type);
                info->number_of_tokens++;
            } else {
            
                size_t length = rest - token_start + 1; 
                incomplete_token = malloc(length + 1);
                if (!incomplete_token) {
                    perror("Malloc failed");
                    exit(EXIT_FAILURE);
                }
                strncpy(incomplete_token, token_start - 1, length);
                incomplete_token[length] = '\0';

                
                rest = token_start + length - 1; 
                break; 
            }
        } else {
        
            char *token_start = rest;
            while (*rest && *rest != ' ' && *rest != '"' && *rest != '\'') {
                rest++;
            }
            size_t length = rest - token_start;
            if (length > 0) {
                char *inner_token = malloc(length + 1);
                if (!inner_token) {
                    perror("Malloc failed");
                    exit(EXIT_FAILURE);
                }
                strncpy(inner_token, token_start, length);
                inner_token[length] = '\0';

                new_token = malloc(sizeof(t_token));
                if (!new_token) {
                    perror("Malloc failed");
                    exit(EXIT_FAILURE);
                }

                new_token->str = inner_token;

            
                if (strcmp(inner_token, ">>") == 0) {
                    new_token->type = APPEND;
                    previous_redirect = 1;
                }
                else if (strcmp(inner_token, ">") == 0) {
                    new_token->type = TRUNC;
                    previous_redirect = 1;
                }
                else if (strcmp(inner_token, "<") == 0) {
                    new_token->type = INPUT;
                    previous_redirect = 1;
                }
                else if (strcmp(inner_token, "|") == 0) {
                    new_token->type = PIPE;
                    previous_redirect = 0;
                    first_token = 1;
                }
                else if (strcmp(inner_token, "&&") == 0 || strcmp(inner_token, "||") == 0 || strcmp(inner_token, "&") == 0) {
                    new_token->type = END;
                    previous_redirect = 0;
                    first_token = 1;
                }
                else if (previous_redirect) {
                    new_token->type = FIL;
                    previous_redirect = 0;
                }
                else if (first_token) {
                    new_token->type = CMD;
                    first_token = 0;
                }
                else {
                    new_token->type = ARG;
                }

                new_token->next = info->tokens_list;
                if (info->tokens_list != NULL) {
                    info->tokens_list->prev = new_token;
                }
                new_token->prev = NULL;
                info->tokens_list = new_token;

                info->number_of_tokens++;
            }
        }
    }


    if (incomplete_token) {

        size_t len = strlen(incomplete_token);
        char *new_token_content = malloc(len + 1);
        if (!new_token_content) {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        char *src = incomplete_token;
        char *dst = new_token_content;
        if (*src == '"' || *src == '\'') {
            *dst++ = *src++;
        }
        while (*src) {
            if (*src == '"' || *src == '\'') {
                *dst++ = *src++; 
            } else {
                *dst++ = *src++;
            }
        }
        *dst = '\0';

        process_incomplete_token(new_token_content, info);
        free(new_token_content);
        free(incomplete_token);
    }
}
