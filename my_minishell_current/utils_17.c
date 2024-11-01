/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_17.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:50:32 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/01 19:26:58 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
char *clean_path(const char *path) {
    char **path_parts;
    char *result;
    int count;
    char *current;
    const char *part;
    const char *start;
    int length;
    int i;

    path_parts = malloc(100 * sizeof(char *));
    if (!path_parts) return NULL;

    count = 0;
    result = malloc(strlen(path) + 1);
    if (!result) {
        free(path_parts);
        return NULL;
    }

    current = result;
    part = path;
    while (*part) {
        if (*part == '/') {
            part++;
            continue;
        }
        start = part;
        while (*part && *part != '/') {
            part++;
        }
        length = part - start;
        if (length > 0) {
            if (strncmp(start, "..", 2) == 0) {
                if (count > 0) {
                    count--;
                }
            } else if (strncmp(start, ".", 1) != 0) {
                path_parts[count] = malloc(length + 1);
                if (path_parts[count]) {
                    memcpy(path_parts[count], start, length);
                    path_parts[count][length] = '\0';
                    count++;
                }
            }
        }
    }

    i = 0;
    while (i < count) {
        if (i > 0) {
            *current = '/';
            current++;
        }
        length = strlen(path_parts[i]);
        memcpy(current, path_parts[i], length);
        current += length;
        free(path_parts[i]);
        i++;
    }
    *current = '\0';

    free(path_parts);
    return result;
}

void if_clean_complex_route(t_section *current) {
    char **env;
    char *pwd;
    char *new_pwd;
    char *new_env_entry;
    int i;

    env = current->info->env;
    pwd = NULL;
    new_pwd = NULL;

    i = 0;
    while (env[i]) {
        if (ft_strncmp_pipex(env[i], "PWD=", 4) == 0) {
            pwd = env[i] + 4;
            break;
        }
        i++;
    }

    if (pwd && strstr(pwd, "..")) {
        new_pwd = clean_path(pwd);
        if (new_pwd) {
            new_env_entry = malloc(strlen(new_pwd) + 5);
            if (new_env_entry) {
                strcpy(new_env_entry, "PWD=");
                strcat(new_env_entry, new_pwd);
                matrix_free(env);
                current->info->env = malloc(2 * sizeof(char *));
                if (current->info->env) {
                    current->info->env[0] = new_env_entry;
                    current->info->env[1] = NULL;
                } else {
                    free(new_env_entry);
                }
            }
            free(new_pwd);
        }
    } else if (new_pwd) {
        free(new_pwd);
    }
}*/

char *clean_path(const char *path) {
    char **path_parts;
    char *result;
    int count;
    char *current;
    const char *part;
    const char *start;
    int length;
    int i;

    path_parts = malloc(100 * sizeof(char *));
    if (!path_parts) return NULL;

    count = 0;
    result = malloc(strlen(path) + 1);
    if (!result) {
        free(path_parts);
        return NULL;
    }

    current = result;
    part = path;
    while (*part) {
        if (*part == '/') {
            part++;
            continue;
        }
        start = part;
        while (*part && *part != '/') {
            part++;
        }
        length = part - start;
        if (length > 0) {
            if (strncmp(start, "..", 2) == 0) {
                if (count > 0) {
                    count--;
                }
            } else if (strncmp(start, ".", 1) != 0) {
                path_parts[count] = malloc(length + 1);
                if (path_parts[count]) {
                    memcpy(path_parts[count], start, length);
                    path_parts[count][length] = '\0';
                    count++;
                }
            }
        }
    }

    i = 0;
    while (i < count) {
        if (i > 0) {
            *current = '/';
            current++;
        }
        length = strlen(path_parts[i]);
        memcpy(current, path_parts[i], length);
        current += length;
        free(path_parts[i]);
        i++;
    }
    *current = '\0';

    free(path_parts);
    return result;
}

void if_clean_complex_route(t_section *current) {
    char **env;
    char *pwd;
    char *new_pwd;
    char *new_env_entry;
    char **new_env;
    int i;
    int env_count;

    env = current->info->env;
    pwd = NULL;
    new_pwd = NULL;

    env_count = 0;
    while (env[env_count]) {
        env_count++;
    }

    new_env = malloc((env_count + 1) * sizeof(char *));
    if (!new_env) return;

    i = 0;
    while (env[i]) {
        if (ft_strncmp_pipex(env[i], "PWD=", 4) == 0) {
            pwd = env[i] + 4;
            if (pwd && strstr(pwd, "..")) {
                new_pwd = clean_path(pwd);
                if (new_pwd) {
                    new_env_entry = malloc(strlen(new_pwd) + 5);
                    if (new_env_entry) {
                        strcpy(new_env_entry, "PWD=");
                        strcat(new_env_entry, new_pwd);
                        new_env[i] = new_env_entry;
                        free(new_pwd);
                    } else {
                        new_env[i] = strdup(env[i]);
                    }
                }
            } else {
                new_env[i] = strdup(env[i]);
            }
        } else {
            new_env[i] = strdup(env[i]);
        }
        i++;
    }
    new_env[i] = NULL;

    matrix_free(env);
    current->info->env = new_env;
}
