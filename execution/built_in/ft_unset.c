/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/27 01:20:39 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	unset_alphanumeric(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
	}
	return (true);
}

bool	check_unset(char *arg, bool flag)
{
	if (ft_isdigit(arg[0]) || !unset_alphanumeric(arg))
	{
		if (flag == true)
			printf(PREFIX_ERROR "unset: not a valid identifier\n");
		return (false);
	}
	return (true);
}

void	check_flag(char *to_update, char ***check, bool flag)
{
	char	*sub;

	if (flag == false)
	{
		sub = ft_substr(to_update, 11, ft_strlen(to_update));
		check_error_null(sub, "malloc");
		*check = ft_split(sub, '=');
		free(sub);
	}
	else
		*check = ft_split(to_update, '=');
	check_error_null(*check, "malloc");
}

void	update_(t_cmd *cmd, char ***to_update, bool flag, char **check)
{
	int		i;
	int		j;
	int		k;
	char	**env;
	char	**tmp;

	(1) && (i = 0, k = 0);
	while (cmd->arguments[++k])
	{
		if (!check_unset(cmd->arguments[k], flag))
			continue ;
		env = malloc(sizeof(char *) * (ft_strdoublelen(*to_update) + 1));
		(1) && (i = -1, j = 0);
		while ((*to_update)[++i])
		{
			check_flag((*to_update)[i], &check, flag);
			if (!(ft_strncmp(check[0], cmd->arguments[k],
						ft_strlen(cmd->arguments[k])) == 0
					&& ft_strlen(check[0]) == ft_strlen(cmd->arguments[k])))
				env[j++] = ft_strdup((*to_update)[i]);
			free_array(check);
		}
		(1) && (env[j] = NULL, tmp = *to_update, *to_update = env);
		free_array(tmp);
	}
}

void	ft_unset(t_cmd *cmd, char ***env, char ***export)
{
	char	**check;

	if (!cmd->arguments[1])
		return ;
	check = NULL;
	update_(cmd, env, true, check);
	update_(cmd, export, false, check);
	if (!cmd->next || !cmd->prev)
		return ;
}
