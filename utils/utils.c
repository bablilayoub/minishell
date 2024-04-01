/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:57:17 by abablil           #+#    #+#             */
/*   Updated: 2024/04/01 00:13:51 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parser.h"
#include "../utils/utils.h"

bool	only_spaces(char *str, t_data *data)
{
	int	i;
	int	count;

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

char	**allocate_empty_env(int flag, t_data *data)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * 5);
	check_error_null(new_env, "malloc");
	if (!flag)
		new_env[0] = ft_strjoin("PWD=", data->temp2);
	else
		new_env[0] = ft_strjoin("declare -x ", data->temp2);
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
		new_env[4]
			= ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	return (new_env[5] = NULL, new_env);
}

char	**allocate_env(char **env, t_data *data)
{
	int		i;
	int		j;
	char	**new_env;

	if (ft_strdoublelen(env) == 0)
	{
		data->empty_env = 1;
		new_env = allocate_empty_env(0, data);
		return (new_env);
	}
	new_env = malloc(sizeof(char *) * (ft_strdoublelen(env) + 1));
	check_error_null(new_env, "malloc");
	(1) && (i = -1, j = 0);
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
	int		flag;
	char	**tmp;

	if (data->empty_env)
		return (allocate_empty_env(1, data));
	flag = 0;
	tmp = start_allocate_export(env, data, &flag);
	if (!flag)
		add_var(&tmp, ft_strjoin("declare -x ", "OLDPWD"), 1, 0);
	return (tmp);
}
