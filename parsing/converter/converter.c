/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/03/19 01:07:06 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	convert_word(t_token **tmp, t_cmd **head, t_cmd **cmd, int *found_cmd)
{
	*found_cmd = 1;
	*cmd = new_cmd(*tmp);
	*head = add_cmd(*head, *cmd);
	if ((*tmp)->next)
		*tmp = find_args(*cmd, (*tmp)->next);
	else
		(*cmd)->args = add_arg((*cmd)->args, (*cmd)->cmd, 1);
}

void	finish_cmd(t_cmd **cmd, t_token **tmp, t_cmd **head)
{
	(*cmd)->built_in = is_built_in((*cmd)->cmd);
	*head = add_cmd(*head, *cmd);
	*tmp = get_command_name(*tmp);
	*tmp = skip_white_spaces(*tmp);
	if (*tmp && (*tmp)->next)
		*tmp = find_args(*cmd, (*tmp)->next);
	else
		(*cmd)->args = add_arg((*cmd)->args, (*cmd)->cmd, 1);
}

void	convert_redirection(t_token **tmp, t_cmd **head,
	t_cmd **cmd, int *found_cmd)
{
	*found_cmd = 1;
	*cmd = new_cmd(*tmp);
	while (*tmp && ((ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0
				|| ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0)
			|| (ft_strncmp((*tmp)->type, HERE_DOC, 2) == 0
				|| ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)))
	{
		if (ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0
			|| ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0
			|| ft_strncmp((*tmp)->type, HERE_DOC, 2) == 0
			|| ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)
			*tmp = add_file(cmd, &(*cmd)->redirects,
					(*tmp)->next, (*tmp)->type);
		if (*tmp)
			*tmp = (*tmp)->next;
		if (*tmp)
			*tmp = skip_white_spaces(*tmp);
	}
	if (*tmp)
		(*cmd)->cmd = (*tmp)->value;
	else
		(*cmd)->cmd = NULL;
	finish_cmd(cmd, tmp, head);
}

void	start_converting(t_token **tmp, t_cmd **head, t_cmd **cmd)
{
	int	found_cmd;

	found_cmd = 0;
	while (*tmp)
	{
		if (ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0
			&& (*tmp)->state != IN_DQUOTE && (*tmp)->state != IN_QUOTE)
			break ;
		if ((ft_strncmp((*tmp)->type, WORD, 4) == 0
				|| ft_strncmp((*tmp)->type, ENV, 1) == 0) && !found_cmd)
		{
			convert_word(tmp, head, cmd, &found_cmd);
			break ;
		}
		else if (cmd_starts_with_redir(*tmp) && !found_cmd)
		{
			convert_redirection(tmp, head, cmd, &found_cmd);
			break ;
		}
		if (*tmp)
			(1 == 1) && (*tmp = (*tmp)->next,
				*tmp = skip_white_spaces(*tmp));
	}
}

void	convert_tokens_to_commands(t_data *data)
{
	t_token	*tmp;
	t_cmd	*head;
	t_cmd	*cmd;

	if (!data->token)
		return ;
	tmp = data->token;
	head = NULL;
	cmd = NULL;
	tmp = clean_tokens(data, tmp);
	tmp = skip_white_spaces(tmp);
	if (!tmp || !check_syntax_error(tmp))
		return ;
	while (tmp)
	{
		cmd = NULL;
		start_converting(&tmp, &head, &cmd);
		if (tmp)
			tmp = tmp->next;
		if (tmp && (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0))
			tmp = tmp->next;
	}
	data->cmd = head;
}
