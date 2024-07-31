/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgimon-c <mgimon-c@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:26:37 by mgimon-c          #+#    #+#             */
/*   Updated: 2024/07/31 17:49:15 by mgimon-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor(t_general *info)
{
    t_section *current = info->sections;
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;

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
			//quizas no necesario
			else if (current->next == NULL && current->fd_write == -1)
				close(pipefd[0]);

            execve(current->path, current->cmdv, NULL);
			write(2, "Error: Command not found\n", 25); 
            exit(127);
        }
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (current->fd_read != -1)
				close(current->fd_read);
            if (current->fd_write != -1 && current->next == NULL)
                close(current->fd_write);
            if (current->next != NULL)
			{
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            current = current->next;
			if (waitpid(pid, &status, 0) == -1)
			{
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status))
                printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
                printf("Process %d was killed by signal %d\n", pid, WTERMSIG(status));
            else if (WIFSTOPPED(status))
                printf("Process %d was stopped by signal %d\n", pid, WSTOPSIG(status));
        }
    }
    while (wait(NULL) > 0);	
}
