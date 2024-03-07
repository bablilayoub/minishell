/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/06 23:13:00 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int args_lenght(t_arg *args)
{
    t_arg *tmp = args;
    
    int i = 0;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

int cmd_lenght(t_cmd *cmd)
{
    t_cmd *tmp = cmd;
    
    int i = 0;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

void close_fds(int fd[][2], int fd_c)
{
    int i;
    int k;

    i = -1;
    while (++i < fd_c)
    {
        k = -1;
        while (++k < 2)
        {
            if (write(fd[i][k], "", 0) == -1)
                continue;
            if (close(fd[i][k]) == -1)
                perror("close");
        }
    }
}

void  update_prefix(t_data *data, char *prefix)
{
    char *new_prefix;
    
    if (!prefix || !prefix[0])
        prefix = "root";
    free(data->prefix);
    new_prefix = ft_strjoin(RESET, BLUE);
    new_prefix = ft_strjoin(new_prefix, BOLD);
    new_prefix = ft_strjoin(new_prefix, "➜  ");
    new_prefix = ft_strjoin(new_prefix, prefix);
    new_prefix = ft_strjoin(new_prefix, " : ");
    new_prefix = ft_strjoin(new_prefix, RESET);
    data->prefix = new_prefix;
}
void close_fds_and_wait(int fd[][2], pid_t pid[], int fd_c, t_data *data)
{
	int i;
	int status;

	status = 0;
	close_fds(fd, fd_c);
	i = -1;
	while (++i < fd_c)
		waitpid(pid[i], &status, 0);
	data->exit_status = WEXITSTATUS(status);
}
void check_error(int fd, const char *msg, int flag)
{
    if (fd == -1)
    {
        perror(msg);
        if (flag)
            return ;
        else
            exit(EXIT_FAILURE);
    }
}

void check_error_null(void *ptr, const char *msg, int flag)
{
    if (!ptr)
    {
        perror(msg);
        if (flag)
            return ;
        else
            exit(EXIT_FAILURE);
    }
}
void free_triplet(char *s1, char *s2, char *s3)
{
    free(s1);
    free(s2);
    free(s3);
}

void free_double(char **ptr)
{
    int i;
    
    i = -1;
    while (ptr[++i])
        free(ptr[i]);
    free(ptr);
}
void free_two(char *s1, char *s2)
{
    if (s1)
        free(s1);
    if (s2)
        free(s2);
}
int ft_strdoublelen(char **str)
{
    int i;

    if (!str)
        return 0;
    i = 0;
    while (str[i])
        i++;
    return i;
}