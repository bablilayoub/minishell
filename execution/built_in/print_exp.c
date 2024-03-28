/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:54:27 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/27 01:22:24 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	sort_export(char **export)
{
	int		i;
	int		k;
	bool	swapped;
	char	*temp;

	k = 11;
	swapped = true;
	while (swapped)
	{
		swapped = false;
		i = 0;
		while (export[i] && export[i + 1])
		{
			if (export[i][k] > export[i + 1][k])
			{
				temp = export[i];
				export[i] = export[i + 1];
				export[i + 1] = temp;
				swapped = true;
			}
			i++;
		}
	}
}

void	copy_to_tmp(char **export, char **tmp, char *temp, char **key_val)
{
	int		i;

	i = -1;
	while (export[++i])
	{
		key_val = key_value(export[i]);
		if (ft_strchr(export[i], '='))
		{
			tmp[i] = ft_strjoin(key_val[0], "=");
			temp = tmp[i];
			tmp[i] = ft_strjoin(tmp[i], "\"");
			(1) && (free(temp), temp = tmp[i]);
			tmp[i] = ft_strjoin(tmp[i], key_val[1]);
			(1) && (free(temp), temp = tmp[i]);
			tmp[i] = ft_strjoin(tmp[i], "\"");
			free(temp);
		}
		else
			tmp[i] = ft_strdup(export[i]);
		free_array(key_val);
	}
	tmp[i] = NULL;
}

void	print(char **export, char *temp)
{
	int		i;
	char	**tmp;
	char	**key_val;

	if (!export || !*export)
		return ;
	key_val = NULL;
	sort_export(export);
	tmp = malloc(sizeof(char *) * (ft_strdoublelen(export) + 1));
	copy_to_tmp(export, tmp, temp, key_val);
	i = -1;
	while (tmp[++i])
		printf("%s\n", tmp[i]);
	free_array(tmp);
}
