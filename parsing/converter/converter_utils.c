/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:46:57 by abablil           #+#    #+#             */
/*   Updated: 2024/03/28 06:13:20 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	not_a_shell_command(t_token *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token->type, PIPE_LINE, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, REDIR_IN, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, REDIR_OUT, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, HERE_DOC, 2) == 0)
		return (0);
	if (ft_strncmp(token->type, APPEND_OUT, 2) == 0)
		return (0);
	return (1);
}

t_token	*get_command_name(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	if (!tmp)
		return (NULL);
	while (tmp && not_a_shell_command(tmp)
		&& ft_strncmp(tmp->type, WORD, 4) != 0)
		tmp = tmp->next;
	return (tmp);
}

int	check_syntax_error(t_token *token, t_data *data)
{
	if (!token)
		return (0);
	if (ft_strncmp(token->type, WORD, 1) != 0
		&& ft_strncmp(token->type, APPEND_OUT, 2) != 0
		&& ft_strncmp(token->type, REDIR_OUT, 1) != 0
		&& ft_strncmp(token->type, HERE_DOC, 2) != 0
		&& ft_strncmp(token->type, REDIR_IN, 1) != 0
		&& ft_strncmp(token->type, ENV, 1) != 0
		&& ft_strncmp(token->type, SPECIAL_CASE, 1) != 0)
	{
		data->exit_status = 258;
		printf("%s\n", PREFIX_ERROR "Syntax error");
		return (0);
	}
	return (1);
}

int	cmd_starts_with_redir(t_token *tmp)
{
	return (ft_strncmp(tmp->type, REDIR_OUT, 1) == 0
		|| ft_strncmp(tmp->type, APPEND_OUT, 2) == 0
		|| ft_strncmp(tmp->type, REDIR_IN, 1) == 0
		|| ft_strncmp(tmp->type, HERE_DOC, 2) == 0);
}
