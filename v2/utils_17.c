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

char	**allocate_path_parts(void)
{
	char	**path_parts;

	path_parts = malloc(100 * sizeof(char *));
	if (!path_parts)
		return (NULL);
	return (path_parts);
}

char	*allocate_result(const char *path, char **path_parts)
{
	char	*result;

	result = malloc(strlen(path) + 1);
	if (!result)
		free(path_parts);
	return (result);
}

void	free_path_parts(char **path_parts, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(path_parts[i]);
		i++;
	}
	free(path_parts);
}

int	handle_part_back(char **path_parts, int count)
{
	if (count > 0)
	{
		free(path_parts[count - 1]);
		return (count - 1);
	}
	return (count);
}

int	process_path_part(char **path_parts,
	int count, const char *start, int length)
{
	path_parts[count] = malloc(length + 1);
	if (!path_parts[count])
		return (-1);
	memcpy(path_parts[count], start, length);
	path_parts[count][length] = '\0';
	return (count + 1);
}

char	*build_result_path(char **path_parts, int count, char *result)
{
	char	*current;
	int		i;
	int		length;

	current = result;
	i = 0;
	while (i < count)
	{
		length = strlen(path_parts[i]);
		if (i > 0)
			*current++ = '/';
		memcpy(current, path_parts[i], length);
		current += length;
		free(path_parts[i]);
		i++;
	}
	*current = '\0';
	free(path_parts);
	return (result);
}

char	*clean_path(const char *path)
{
	char		**path_parts;
	char		*result;
	int			count;
	int			length;
	const char	*part;
	const char	*start;

	path_parts = allocate_path_parts();
	if (!path_parts)
		return (NULL);
	result = allocate_result(path, path_parts);
	if (!result)
		return (NULL);
	count = 0;
	part = path;
	while (*part)
	{
		if (*part == '/')
		{
			part++;
			continue ;
		}
		start = part;
		while (*part && *part != '/')
			part++;
		length = part - start;
		if (length > 0)
		{
			if (strncmp(start, "..", 2) == 0)
				count = handle_part_back(path_parts, count);
			else if (strncmp(start, ".", 1) != 0)
			{
				count = process_path_part(path_parts, count, start, length);
				if (count == -1)
				{
					free_path_parts(path_parts, count);
					free(result);
					return (NULL);
				}
			}
		}
	}
	return (build_result_path(path_parts, count, result));
}

char	**allocate_env(int env_count)
{
	char	**new_env;

	new_env = malloc((env_count + 1) * sizeof(char *));
	return (new_env);
}

void	copy_env_entry(char **new_env, char **env, int i)
{
	new_env[i] = strdup(env[i]);
}

char	*generate_new_pwd_entry(char *new_pwd)
{
	char	*new_env_entry;

	new_env_entry = malloc(strlen(new_pwd) + 5);
	if (new_env_entry)
	{
		strcpy(new_env_entry, "PWD=");
		strcat(new_env_entry, new_pwd);
	}
	free(new_pwd);
	return (new_env_entry);
}

void	update_pwd_entry(char **new_env, char **env, int i, char *pwd)
{
	char	*new_pwd;

	new_pwd = clean_path(pwd);
	if (new_pwd)
	{
		new_env[i] = generate_new_pwd_entry(new_pwd);
		if (!new_env[i])
			copy_env_entry(new_env, env, i);
	}
	else
		copy_env_entry(new_env, env, i);
}

char	**create_updated_env(char **env)
{
	int		env_count;
	char	**new_env;

	env_count = 0;
	while (env[env_count])
		env_count++;
	new_env = allocate_env(env_count);
	if (!new_env)
		return (NULL);
	return (new_env);
}

void	if_clean_complex_route(t_section *current)
{
	int		i;
	char	**env;
	char	**new_env;
	char	*pwd;

	i = 0;
	env = current->info->env;
	new_env = create_updated_env(env);
	if (!new_env)
		return ;
	while (env[i])
	{
		if (strncmp(env[i], "PWD=", 4) == 0)
		{
			pwd = env[i] + 4;
			if (pwd && strstr(pwd, ".."))
				update_pwd_entry(new_env, env, i, pwd);
			else
				copy_env_entry(new_env, env, i);
		}
		else
			copy_env_entry(new_env, env, i);
		i++;
	}
	new_env[i] = NULL;
	matrix_free(env);
	current->info->env = new_env;
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

void	free_split_pwd(char **split_pwd)
{
	int	j;

	j = 0;
	while (split_pwd[j] != NULL)
	{
		free(split_pwd[j]);
		j++;
	}
	free(split_pwd);
}

char	*build_new_pwd(char **split_pwd, int new_len)
{
	char	*new_pwd;
	int		j;
	size_t	needed_size;

	new_pwd = malloc(5 + 1);
	j = 0;
	strcpy(new_pwd, "PWD=");
	while (j < new_len)
	{
		needed_size = strlen(new_pwd) + strlen(split_pwd[j]) + 2;
		new_pwd = realloc(new_pwd, needed_size);
		strcat(new_pwd, split_pwd[j]);
		if (j < new_len - 1)
			strcat(new_pwd, "/");
		j++;
	}
	return (new_pwd);
}

int	find_pwd_index(t_section *current)
{
	int	i;

	i = 0;
	while (current->info->env[i] != NULL)
	{
		if (ft_strncmp_pipex(current->info->env[i], "PWD=", 4) == 0)
			break ;
		i++;
	}
	return (i);
}

int	calculate_new_len(char **split_pwd, int dirs_back)
{
	int	new_len;

	new_len = 0;
	while (split_pwd[new_len] != NULL)
		new_len++;
	if (dirs_back >= new_len)
		return (0);
	return (new_len - dirs_back);
}

char	**rebuild_env(t_section *current,
	char *new_pwd, int env_size, int pwd_index)
{
	char	**new_env;
	int		k;

	new_env = malloc((env_size + 1) * sizeof(char *));
	k = 0;
	while (k < env_size)
	{
		if (k == pwd_index)
			new_env[k] = new_pwd;
		else
			new_env[k] = strdup(current->info->env[k]);
		k++;
	}
	new_env[k] = NULL;
	return (new_env);
}

void	free_old_env(char **env)
{
	int	j;

	j = 0;
	while (env[j] != NULL)
	{
		free(env[j]);
		j++;
	}
	free(env);
}

void remove_backdirs(t_section *current, int dirs_back)
{
	int			pwd_index;
	int			new_len;
	int			env_size;
	char		*pwd_line;
	char		*new_pwd;
	char		**split_pwd;
	char		**new_env;

	if (dirs_back < 1)
		return ;
	pwd_index = find_pwd_index(current);
	if (current->info->env[pwd_index] == NULL)
		return ;
	pwd_line = current->info->env[pwd_index] + 4;
	if (strlen(pwd_line) == 0)
		return ;
	split_pwd = ft_split(pwd_line, '/');
	new_len = calculate_new_len(split_pwd, dirs_back);
	new_pwd = build_new_pwd(split_pwd, new_len);
	env_size = 0;
	while (current->info->env[env_size] != NULL)
		env_size++;
	new_env = rebuild_env(current, new_pwd, env_size, pwd_index);
	free_old_env(current->info->env);
	current->info->env = new_env;
	free_split_pwd(split_pwd);
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
