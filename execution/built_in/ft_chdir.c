/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:09 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/28 05:56:31 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

char	**update_exppwd(char **exp, char *oldpwd, char *pwd)
{
	int		i;
	char	*tmp;

	i = -1;
	while (exp[++i])
	{
		tmp = exp[i];
		if (ft_strncmp(exp[i], "declare -x OLDPWD", 17) == 0)
			exp[i] = ft_strjoin("declare -x OLDPWD=", oldpwd);
		else if (ft_strncmp(exp[i], "declare -x PWD", 14) == 0)
			exp[i] = ft_strjoin("declare -x PWD=", pwd);
		else
			exp[i] = ft_strdup(exp[i]);
		free(tmp);
		check_error_null(exp[i], "malloc");
	}
	exp[i] = NULL;
	return (exp);
}

char	**update_envpwd(char **env, char *oldpwd, char *pwd)
{
	int			i;
	char		*tmp;
	static int	set = 0;

	i = -1;
	if (!set)
	{
		add_var(&env, ft_strjoin("OLDPWD=", oldpwd), 1, 0);
		set = 1;
	}
	while (env[++i])
	{
		tmp = env[i];
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
			env[i] = ft_strjoin("OLDPWD=", oldpwd);
		else if (ft_strncmp(env[i], "PWD", 3) == 0)
			env[i] = ft_strjoin("PWD=", pwd);
		else
			env[i] = ft_strdup(env[i]);
		free(tmp);
		check_error_null(env[i], "malloc");
	}
	env[i] = NULL;
	return (env);
}

char	*ft_getenv(char *name, char **env)
{
	int	i;
	int	j;

	if (!env || !name)
		return (NULL);
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

void	start_process(t_data *data, char *oldpwd, char *pwd, char ***env)
{
	char	*new_prefix;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (check_error_null(pwd, "getcwd"));
	*env = update_envpwd(*env, oldpwd, pwd);
	data->export = update_exppwd(data->export, oldpwd, pwd);
	if (!*env)
	{
		perror("malloc");
		free_two(oldpwd, pwd);
		exit(EXIT_FAILURE);
	}
	new_prefix = ft_strrchr(pwd, '/');
	update_prefix(data, new_prefix + 1);
	free_two(oldpwd, pwd);
}

void	ft_chdir(t_cmd *cmd, t_data *data)
{
	char	*dirname;

	(1 == 1) && (data->oldpwd = NULL, data->pwd = NULL, dirname = NULL);
	data->oldpwd = getcwd(NULL, 0);
	dirname = cmd->arguments[1];
	if (!data->oldpwd)
		return (check_directory("getcwd", dirname, data->env));
	if (dirname && ft_strlen(dirname) == 0)
		return (free(data->oldpwd));
	if (!dirname || (dirname[0] == '~' && dirname[1] == '\0'))
	{
		if (chdir(ft_getenv("HOME", data->env)) == 0)
			return (start_process(data, data->oldpwd, data->pwd, &data->env));
		else
			return (cd_failed(cmd, data->oldpwd, data));
	}
	if (access(dirname, F_OK) == 0)
	{
		if (chdir(dirname) == 0)
			return (start_process(data, data->oldpwd, data->pwd, &data->env));
		else
			return (print_error(cmd, dirname, data->oldpwd, data));
	}
	else
		return (print_error(cmd, dirname, data->oldpwd, data));
}
