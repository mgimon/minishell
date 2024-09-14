/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/09/14 21:25:53 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ultimo else if quizas no necesario
// si no ejecuta builtin, hara el execve
// si el execve no termina el proceso, gestiona el error cmdnotfound
void	child_process(t_section *current, int prev_fd, int *pipefd)
{
	write_in_heredocs(current);
	printf("El fd read es %d\n", current->fd_read);
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
/*	for (int i = 3; i < 20; i++)
		close(i);*/
	if (exec_if_builtin_1(current) == 0)
	{
		free_sections_list(current->info->sections);
		exit(0);
	}
	fprintf(stderr, "El fd write es %d\n", current->fd_write);
	execve(current->path, current->cmdv, current->info->env);
	current->gottofree = 1;
	put_str_fd(2, current->cmdv[0]);
	put_str_fd(2, ": Command not found\n");
	exit(127);
}

void	parent_process(t_section **current, int *prev_fd, int *pipefd, pid_t pid, int status)
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
	for (int i = 3; i < 20; i++)
		close(i);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (WIFEXITED(status))
	{
		printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
		(*current)->info->exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		printf("Process %d was killed by signal %d\n", pid, WTERMSIG(status));
		(*current)->info->exit_status = WTERMSIG(status);
	}
	else if (WIFSTOPPED(status))
	{
		printf("Process %d was stopped by signal %d\n", pid, WSTOPSIG(status));
		(*current)->info->exit_status = WSTOPSIG(status);
	}
	exec_if_builtin_2(*current);
	*current = (*current)->next;
}

void	executor(t_general *info)
{
    t_section *current;
    int pipefd[2];
    int prev_fd;
    pid_t pid;
    int status;

	status = 0;
	prev_fd = -1;
	current = info->sections;
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
        pid = fork();
        if (pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
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
			parent_process(&current, &prev_fd, pipefd, pid, status);
		}
    }
}
