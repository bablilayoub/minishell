/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 01:56:46 by alaalalm          #+#    #+#             */
/*   Updated: 2024/04/11 14:16:32 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	get_file(t_cmd *cmd, char *file)
{
	int	fd;

	fd = 0;
	if (access(file, F_OK) == -1)
	{
		if (!cmd->built_in)
			exit(EXIT_FAILURE);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		printf(PREFIX_ERROR "%s: %s\n", file, strerror(errno));
		if (!cmd->built_in)
			exit(EXIT_FAILURE);
		return (-1);
	}
	return (fd);
}

void	ft_pipe(int **fd)
{
	int	i;

	i = -1;
	while (fd[++i])
	{
		if (pipe(fd[i]) == -1)
			perror("pipe");
	}
}

void	check_redirects( t_cmd *current, pid_t pid, t_data *data)
{
	t_redirection		*tmp;

	tmp = current->redirects;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "<<", 2) == 0)
			waitpid(pid, &data->exit_status, 0);
		tmp = tmp->next;
	}
}

bool	between_dquotes(t_token *token)
{
	t_token	*tmp;
	int		count;

	count = 0;
	tmp = token;
	while (tmp && ft_strncmp(tmp->type, QUOTE, 1) != 0)
	{
		if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			count++;
		tmp = tmp->prev;
	}
	return (count % 2 == 0);
}

void	get_parent_pid(t_data *data)
{
	int	ret;

	ret = fork();
	if (ret == -1)
	{
		printf(PREFIX_ERROR"Error: fork failed\n");
		exit(1);
	}
	if (ret == 0)
		exit(0);
	data->parent_pid = ret - 1;
}
