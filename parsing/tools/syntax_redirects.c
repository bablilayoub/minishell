/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:40:03 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 18:37:46 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	handle_expandable_file(t_redirection **tmp, t_data *data)
{
	data->temp = (*tmp)->file;
	(*tmp)->file = get_env((*tmp)->file + 1, data);
	free(data->temp);
	if ((*tmp)->file && access((*tmp)->file, F_OK) == -1
		&& ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)
	{
		printf("%s\n", PREFIX_ERROR "No such file or directory");
		return (0);
	}
	else if (!(*tmp)->file)
	{
		printf("%s\n", PREFIX_ERROR "No such file or directory");
		(*tmp)->file = ft_strdup("");
		return (0);
	}
	return (1);
}

int	not_file(char *file)
{
	if (!file)
	{
		printf("%s\n", PREFIX_ERROR "syntax error, wrong redirection");
		return (0);
	}
	return (1);
}

int	validate_redirects(t_redirection **redirects, t_data *data)
{
	t_redirection	*tmp;

	tmp = *redirects;
	while (tmp)
	{
		if (!not_file(tmp->file))
			return (0);
		if (ft_strncmp(tmp->type, HERE_DOC, 2) != 0)
		{
			if (tmp->file[0] == '$'
				&& tmp->file[1] != '\0' && tmp->expandalbe)
			{
				if (!handle_expandable_file(&tmp, data))
					return (0);
			}
			else if (access(tmp->file, F_OK) == -1
				&& ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
			{
				printf("%s\n", PREFIX_ERROR "No such file or directory");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}
