/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_17.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:50:32 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/11/01 21:24:38 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *clean_path(const char *path)
{
    char **path_parts;
    char *result;
    int count = 0;
    char *current;
    const char *part;
    const char *start;
    int length;
    int i;

    path_parts = malloc(100 * sizeof(char *)); // Allocate memory for path parts
    if (!path_parts) return NULL; // Check allocation

    result = malloc(strlen(path) + 1); // Allocate memory for the result
    if (!result) {
        free(path_parts);
        return NULL; // Check allocation
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
                // Handle '..' by decrementing count
                if (count > 0) {
                    count--;
                    free(path_parts[count]); // Free the last valid part
                }
            } else if (strncmp(start, ".", 1) != 0) {
                path_parts[count] = malloc(length + 1); // Allocate memory for each part
                if (!path_parts[count]) {
                    // Free previously allocated parts if malloc fails
                    for (i = 0; i < count; i++) {
                        free(path_parts[i]);
                    }
                    free(path_parts);
                    free(result);
                    return NULL; // Handle allocation failure
                }
                memcpy(path_parts[count], start, length);
                path_parts[count][length] = '\0'; // Null terminate
                count++;
            }
        }
    }

    // Constructing the final result path
    for (i = 0; i < count; i++) {
        if (i > 0) {
            *current = '/';
            current++;
        }
        length = strlen(path_parts[i]);
        memcpy(current, path_parts[i], length);
        current += length;
        free(path_parts[i]); // Free each part after using it
    }
    *current = '\0'; // Null terminate result

    free(path_parts); // Free the array holding the parts
    return result; // Return the cleaned path
}

void if_clean_complex_route(t_section *current)
{
    char **env = current->info->env;
    char *pwd = NULL;
    char *new_pwd = NULL;
    char *new_env_entry;
    char **new_env;
    int i = 0;
    int env_count = 0;

    // Calculate the environment size
    while (env[env_count]) {
        env_count++;
    }

    new_env = malloc((env_count + 1) * sizeof(char *));
    if (!new_env) return; // Check allocation failure

    while (env[i]) {
        if (strncmp(env[i], "PWD=", 4) == 0) {
            pwd = env[i] + 4; // Extract current PWD value
            if (pwd && strstr(pwd, "..")) {
                new_pwd = clean_path(pwd); // Call clean_path
                if (new_pwd) {
                    new_env_entry = malloc(strlen(new_pwd) + 5); // Allocate new_env_entry
                    if (new_env_entry) {
                        strcpy(new_env_entry, "PWD=");
                        strcat(new_env_entry, new_pwd);
                        new_env[i] = new_env_entry;
                        free(new_pwd); // Free new_pwd after usage
                    } else {
                        new_env[i] = strdup(env[i]); // Fallback
                    }
                }
            } else {
                new_env[i] = strdup(env[i]); // Duplicate the environment variable if not changed
            }
        } else {
            new_env[i] = strdup(env[i]); // Duplicate other environment variables
        }
        i++;
    }
    new_env[i] = NULL; // Null terminate new environment

    matrix_free(env); // Free old environment
    current->info->env = new_env; // Set new environment
}

int	is_backdirs_only(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != '.' && str[i] != '/')
			return (0);
		if (str[i] == '.')
			count++;
		i++;
	}
	if (count > 3)
		return (count);
	return (0);
}

void	remove_backdirs(t_section *current, int dirs_back)
{
    int i;
    char *pwd_line;
    char *new_pwd;
    char **split_pwd;
    char **new_env;
    int j;
    int new_len;
    int k;
    int env_size;

    i = 0;
	if (dirs_back < 1)
		return ;
    while (current->info->env[i] != NULL)
    {
        if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
            break;
        i++;
    }
    if (current->info->env[i] == NULL)
        return;

    pwd_line = current->info->env[i] + 4;
    if (strlen(pwd_line) == 0)
        return;

    split_pwd = ft_split(pwd_line, '/');
    new_len = 0;
    while (split_pwd[new_len] != NULL)
        new_len++;
    if (dirs_back >= new_len)
        new_len = 0;
    else
        new_len -= dirs_back;

    new_pwd = malloc(5 + 1);
    strcpy(new_pwd, "PWD=");
    for (j = 0; j < new_len; j++)
    {
        size_t needed_size = strlen(new_pwd) + strlen(split_pwd[j]) + 2;
        new_pwd = realloc(new_pwd, needed_size);
        strcat(new_pwd, split_pwd[j]);
        if (j < new_len - 1)
            strcat(new_pwd, "/");
    }

    env_size = 0;
    while (current->info->env[env_size] != NULL)
        env_size++;

    new_env = malloc((env_size + 1) * sizeof(char *));
    k = 0;
    while (k < env_size)
    {
        if (k == i)
            new_env[k] = new_pwd;
        else
            new_env[k] = strdup(current->info->env[k]);
        k++;
    }
    new_env[k] = NULL;

    for (j = 0; current->info->env[j] != NULL; j++)
        free(current->info->env[j]);
    free(current->info->env);
    current->info->env = new_env;

    j = 0;
    while (split_pwd[j] != NULL)
    {
        free(split_pwd[j]);
        j++;
    }
    free(split_pwd);
}

void	if_backdirs_only(t_section *current)
{
	int	i;
	int	dirs_back;

	if (!is_backdirs_only(current->cmdv[1]))
		return ;
	i = is_backdirs_only(current->cmdv[1]);
	dirs_back = (i / 2) - 1;
	if (dirs_back > 1)
		dirs_back--;
	remove_backdirs(current, dirs_back);
}
