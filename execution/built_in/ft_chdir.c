/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:09 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/10 22:44:15 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

char **update_envpwd(char **env, char *oldpwd, char *pwd)
{
    int i;

    i = -1;
    while (env[++i])
    {
        if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
            env[i] = ft_strjoin("OLDPWD=", oldpwd);
        else if (ft_strncmp(env[i], "PWD", 3) == 0)
            env[i] = ft_strjoin("PWD=", pwd);
        else
            env[i] = ft_strdup(env[i]);

    }
    env[i] = NULL;
    return (env);
}

char *ft_getenv(char *name, char **env)
{
    int i;
    int j;

    if (!env || !name)
        return NULL;
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

void start_process(t_data *data, char *oldpwd, char *pwd, char ***env)
{
    char *new_prefix;
    t_cmd *cmd;
    
    cmd = data->cmd;
    pwd = getcwd(NULL, 0);
    if (!pwd)
        return check_error_null(pwd, "getcwd", cmd);
    *env = update_envpwd(*env, oldpwd, pwd);
    new_prefix = ft_strrchr(pwd, '/');
    update_prefix(data, new_prefix + 1);
    if (cmd->next || cmd->prev)
        exit(EXIT_SUCCESS);
    else
        return;
}
void ft_chdir(t_cmd *cmd, t_data *data)
{
    char *dirname;
    char *oldpwd;
    char *pwd;
    
    (1 == 1) && (oldpwd = NULL, pwd = NULL, dirname = NULL);
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
        return check_error_null(oldpwd, "getcwd", cmd);
    dirname = cmd->arguments[1];
    if (!dirname || (dirname[0] == '~' && dirname[1] == '\0'))
    {
        if (chdir(ft_getenv("HOME", data->env)) == 0)
            return start_process(data, oldpwd, pwd, &data->env);
        else
        {
            printf(PREFIX_ERROR "HOME not set\n");
            if (cmd->next || cmd->prev)
                exit(EXIT_FAILURE);
            else
                return;
        }
    }
    if (access(dirname, F_OK) == 0)
    {
        if (chdir(dirname) == 0)
            return start_process(data, oldpwd, pwd, &data->env);
        else
          return print_error(cmd, dirname);
    }
    else
       return print_error(cmd, dirname);
}
