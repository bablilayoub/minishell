/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:57:17 by abablil           #+#    #+#             */
/*   Updated: 2024/03/27 23:53:14 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parser.h"

bool    only_spaces(char *str, t_data *data)
{
    int    i;
    int    count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            count++;
        i++;
    }
    if (count == i)
    {
        data->exit_status = 0;
        return (true);
    }
    return (false);
}
void	increment_shell_lvl(char **env, int *i, t_data *data)
{
	char	*shell_lvl;
	char	**key_val;
	char	*lvl;

	key_val = ft_split(env[*i], '=');
	shell_lvl = ft_substr(*env, 6, ft_strlen(*env));
	check_error_null(shell_lvl, "malloc");
	lvl = ft_itoa(ft_atoi(key_val[1]) + 1);
	data->temp = shell_lvl;
	if (ft_atoi(key_val[1]) != 999)
		shell_lvl = ft_strjoin("SHLVL=", lvl);
	else
		shell_lvl = ft_strjoin("SHLVL=", "");
	free(data->temp);
	free_array(key_val);
	free(lvl);
	env[*i] = shell_lvl;
		
}
char	**allocate_empty_env(int flag)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * 5);
	check_error_null(new_env, "malloc");
	if (!flag)
		new_env[0] = ft_strjoin("PWD=", getcwd(NULL, 0));
	else
		new_env[0] = ft_strjoin("declare -x ", getcwd(NULL, 0));
	check_error_null(new_env[0], "malloc");
	if (!flag)
		new_env[1] = ft_strjoin("SHLVL=", "1");
	else
		new_env[1] = ft_strjoin("declare -x ", "SHLVL=1");
	check_error_null(new_env[1], "malloc");
	if (!flag)
		new_env[2] = ft_strdup("_=/usr/bin/env");
	else
		new_env[2] = ft_strjoin("declare -x ", "_=/usr/bin/env");
	check_error_null(new_env[2], "malloc");
	if (!flag)
		new_env[3] = ft_strjoin("SHELL=", "/bin/zsh");
	if (!flag)	
		new_env[4] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	new_env[5] = NULL;
	return (new_env);
}
char	**allocate_env(char **env, t_data *data)
{
	int		i;
	int		j;
	char	**new_env;

	if (ft_strdoublelen(env) == 0)
	{
		data->empty_env = 1;
		new_env = allocate_empty_env(0);
		return (new_env);
	}
	new_env = malloc(sizeof(char *) * (ft_strdoublelen(env) + 1));
	check_error_null(new_env, "malloc");
	i = -1;
	j = 0;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
			continue ;
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			increment_shell_lvl(env, &i, data);
		new_env[j] = ft_strdup(env[i]);
		check_error_null(new_env[j], "malloc");
		j++;
	}
	new_env[j] = NULL;
	return (new_env);
}

char	**allocate_export(char **env, t_data *data)
{
	int		i;
	int		flag;
	int		lenght;
	char	**tmp;

	if (data->empty_env)
		return (allocate_empty_env(1));
	lenght = ft_strdoublelen(env);
	tmp = malloc(sizeof(char *) * (lenght + 1));
	check_error_null(tmp, "malloc");
	(1) && (i = -1, flag = 0);
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
		{
			tmp[i] = ft_strjoin("declare -x ", "OLDPWD");
			flag = 1;
		}
		else
		{
			if (ft_strncmp(env[i], "declare -x SHLVL=", 17) == 0)
				increment_shell_lvl(env, &i, data);
			tmp[i] = ft_strjoin("declare -x ", env[i]);
			check_error_null(tmp[i], "malloc");
		}
	}
	tmp[i] = NULL;
	if (!flag)
		add_var(&tmp, ft_strjoin("declare -x ", "OLDPWD"), 1, 0);
	return (tmp);
}

