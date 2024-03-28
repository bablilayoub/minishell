/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 00:18:53 by abablil           #+#    #+#             */
/*   Updated: 2024/03/28 00:19:31 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parser.h"
#include "../utils/utils.h"

char	**start_allocate_export(char **env, t_data *data, int *flag)
{
	int		i;
	char	**tmp;

	i = -1;
	tmp = malloc(sizeof(char *) * (ft_strdoublelen(env) + 1));
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
		{
			tmp[i] = ft_strjoin("declare -x ", "OLDPWD");
			*flag = 1;
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
	return (tmp);
}
