/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/17 20:39:16 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ultimo else if quizas no necesario
// si no ejecuta builtin, hara el execve
// si el execve no termina el proceso, gestiona el error cmdnotfound
void	child_process(t_section *current, int prev_fd, int *pipefd)
{
	write_in_heredocs(current);
	if (current->fd_read != -1)
	{
		dup2(current->fd_read, STDIN_FILENO);
		close(current->fd_read);
	}
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->fd_write != -1)
	{
		dup2(current->fd_write, STDOUT_FILENO);
		close(current->fd_write);
	}
	else if (current->next != NULL)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	else if (current->next == NULL && current->fd_write == -1)
		close(pipefd[0]);
	if (exec_if_builtin_1(current) == 0)
	{
		free_files(current->files);
		free_sections_list(current->info->sections);
		exit(0);
	}
	execve(current->path, current->cmdv, current->info->env);
	current->gottofree = 1;
	put_str_fd(2, current->cmdv[0]);
	put_str_fd(2, ": Command not found\n");
	exit(127);
}

//close_all_process_files();
void	parent_process(t_section **current, int *prev_fd, int *pipefd)
{
	close_section_hdocs_parent(*current);
	if (*prev_fd != -1)
		close(*prev_fd);
	if ((*current)->fd_read != -1)
		close((*current)->fd_read);
	if ((*current)->fd_write != -1 && (*current)->next == NULL)
		close((*current)->fd_write);
	if ((*current)->next != NULL)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	exec_if_builtin_2(*current);
	free_files((*current)->files);
	*current = (*current)->next;
}

void	executor(t_general *info)
{
    t_section *current;
    t_section *tmp_sec;
    int pipefd[2];
    int prev_fd;
    int status;

	status = 0;
	prev_fd = -1;
	current = info->sections;
	tmp_sec = info->sections;
    while (current != NULL)
	{
        if (current->next != NULL)
		{
            if (pipe(pipefd) == -1)
			{
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        current->pid = fork();
        if (current->pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (current->pid == 0)
		{
			if (current->next == NULL)
			{
				pipefd[0] = -1;
				pipefd[1] = -1;
			}
			child_process(current, prev_fd, pipefd);
		}
		else
		{
			parent_process(&current, &prev_fd, pipefd);
		}
    }
	while (tmp_sec)
	{
		waitpid(tmp_sec->pid, &status, 0);
		tmp_sec = tmp_sec->next;
	}
    if (WIFEXITED(status))
    {
        printf("Process exited with status %d\n", WEXITSTATUS(status));
        info->exit_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process was killed by signal %d\n", WTERMSIG(status));
        info->exit_status = WTERMSIG(status);
    }
    else if (WIFSTOPPED(status))
    {
        printf("Process was stopped by signal %d\n", WSTOPSIG(status));
        info->exit_status = WSTOPSIG(status);
    }
}
