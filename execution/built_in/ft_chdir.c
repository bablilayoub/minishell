/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:09 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/05 23:24:15 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

char *ft_getenv(char *name, char **env)
{
    int i;
    int j;

    i = -1;
    while (env[++i])
    {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        if (ft_strncmp(env[i], name, j) == 0)
            return (env[i] + j + 1);
    }
    return (NULL);
}
void ft_chdir(t_cmd *cmd, t_data *data)
{
    const char  *dirname;
    int         fd_out;

    fd_out = open(ft_strjoin(data->shell_path, "/cd.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    check_error(fd_out, "open", 0);
    dirname = cmd->arguments[1];
    if (!dirname || (dirname[0] == '~' && dirname[1] == '\0'))
    {
        if (chdir(ft_getenv("HOME", data->env)) == 0)
        {
            write(fd_out, ft_getenv("HOME", data->env), ft_strlen(ft_getenv("HOME", data->env)));
            close(fd_out);
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_FAILURE);
    }
    if (access(dirname, F_OK) == 0)
    {
        if (chdir(dirname) == 0)
            write(fd_out, dirname, ft_strlen(dirname));
    }
    close(fd_out);
    exit(EXIT_FAILURE);
}
