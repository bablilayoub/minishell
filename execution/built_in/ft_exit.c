/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:21 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/03 02:47:08 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_exit(t_data *data, t_cmd *cmd)
{
    int fd_out;

    fd_out = open(ft_strjoin(data->shell_path, "/exit.txt"), O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (cmd->arguments[1] == NULL)
    {
        write(fd_out, "exit", 4);
        close(fd_out);
        exit(EXIT_SUCCESS);
    }
    else
    {
        write(fd_out, cmd->arguments[1], ft_strlen(cmd->arguments[1]));
        close(fd_out);
        exit(atoi(cmd->arguments[1]));
    }
}