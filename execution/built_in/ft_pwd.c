/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:38 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/13 16:56:16 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_pwd(t_data *data)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        printf("%s\n", strerror(errno));
        free_data(data, 1);
        exit(1);
    }
    printf("%s\n", path);
    free(path);
    if (!data->cmd->next || !data->cmd->prev)
        return;
}