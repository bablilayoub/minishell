/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:46:52 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/28 21:02:59 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int check_exported(char *exported)
{
    int i;

    i = 0;
    if (!ft_strchr(exported, '='))
        return false;
    else if ((exported[i] == '=' && exported[i + 1] == '\0'))
        return (printf(PREFIX_ERROR "bad assignment\n"), false);
    else if (exported[i] == '=' && ft_isascii(exported[i + 1]))
        return (printf(PREFIX_ERROR "not found\n"), false);
    else if (ft_isdigit(exported[i]))
        return (printf(PREFIX_ERROR "export: not an identifier\n"), false);
    return true;
}

char **load_key_value(char *key, char *value)
{
    char **key_value;
    int i;
    
    i = 0;
    key_value = malloc(sizeof(char *) * 3);
    if (!key_value)
    {
        free(key);
        free(value);
        exit(EXIT_FAILURE);
    }
    key_value[0] = key;
    key_value[1] = value;
    key_value[2] = NULL;
    return key_value;
}

// char *check_dollar_sign(char **env, char *exported)
// {
//     int i;
//     int k;
//     int j;
//     char *temp;
//     char *value;

//     i = 0;
//     while (exported[i])
//     {
//         if (exported[i] == '$')
//         {
//             value = ft_substr(exported, 0, i);
//             k = i;
//             while (exported[k] && (exported[k] != ' ' && exported[k] != '='))
//                 k++;
//             temp = ft_substr(exported, i, k);
//             if (!temp)
//                 exit(EXIT_FAILURE);
//             j = 0;
//             while (env[j])
//             {
//                 if (ft_strncmp(env[j], temp, ft_strlen(temp)) == 0)
//                     value = ft_strjoin(value, env[j] + ft_strlen(temp));
//                 j++;
//             }
//             if (!env[j])
//             {
//                 printf(PREFIX_ERROR "export: not found\n");
//                 free(temp);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         i++;
//     }
//     return value;
// }
char **key_value(char *exported, char **env)
{
    (void)env;
    char *value;
    char *key;
    char **key_value;
    int i;
    int k;

    i = 0;
    while (exported[i] && exported[i] != '=')
        i++;
    key = ft_substr(exported, 0, i);
    if (!key)
        exit(EXIT_FAILURE);
    k = i;
    while (exported[k])
        k++;
    value = ft_substr(exported, i + 1, k);
    // value = check_dollar_sign(env, value);
    if (!value)
    {
        free(key);
        exit(EXIT_FAILURE);
    }
    key_value = load_key_value(key, value);
    return key_value;
}
void ret_same_env(char **env)
{
    int i;
    int fd_out;

    i = 0;
    fd_out = open("export.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (fd_out == -1)
        return check_error(fd_out, "open", 0);
    while (env[i])
    {
        write(fd_out, env[i], ft_strlen(env[i]));
        write(fd_out, "\n", 1);
        i++;
    }
    close(fd_out);
    exit(EXIT_FAILURE);
}

void add_variable(int fd_out, char *exported, int *found)
{
    if (*found == 1)
        return ;
    write(fd_out, exported, ft_strlen(exported));
    write(fd_out, "\n", 1);
}

void update_key_value(int fd_out, char *key, char *value, int *found)
{
    write(fd_out, key, ft_strlen(key));
    write(fd_out, "=", 1);
    write(fd_out, value, ft_strlen(value));
    write(fd_out, "\n", 1);
    *found = 1;
}
void ft_export(t_data *data, char **env)
{
    char **key_val;
    int fd_out;
    int found;
    size_t k;
    int i;

    if (!check_exported(data->cmd->arguments[1]))
        ret_same_env(data->env);
    key_val = key_value(data->cmd->arguments[1], env);
    fd_out = open("export.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (fd_out == -1)
        return check_error(fd_out, "open", 0);
    (1 == 1) && (i = -1, found = 0);
    while (env[++i])
    {
        k = 0;
        while (env[i][k] && env[i][k] != '=')
            k++;
        if ((ft_strncmp(env[i], key_val[0], k) == 0) && (k == ft_strlen(key_val[0])))
            update_key_value(fd_out, key_val[0], key_val[1], &found);
        else
            write(fd_out, env[i], ft_strlen(env[i]));
        write(fd_out, "\n", 1);
    }
    add_variable(fd_out, data->cmd->arguments[1], &found);
    close(fd_out);
}