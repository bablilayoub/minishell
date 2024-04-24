/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/04/23 16:44:59 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	convert_word(t_token **tmp, t_cmd **head, t_cmd **cmd, int *found_cmd)
{
	*found_cmd = 1;
	if ((*tmp)->value && ft_strlen((*tmp)->value) > 0)
		*cmd = new_cmd(*tmp);
	else
	{
		free((*tmp)->value);
		(*tmp)->value = ft_strdup(" ");
		free((*tmp)->type);
		(*tmp)->type = ft_strdup(SPECIAL_CASE);
		*cmd = new_cmd(*tmp);
	}
	*head = add_cmd(*head, *cmd);
	if ((*tmp)->next)
		*tmp = find_args(*cmd, (*tmp)->next);
	else
		(*cmd)->args = add_arg((*cmd)->args, (*cmd)->cmd, 1);
}

void	finish_cmd(t_cmd **cmd, t_token **tmp, t_cmd **head)
{
	if ((*cmd)->cmd && ft_strlen((*cmd)->cmd) == 0)
	{
		free((*cmd)->cmd);
		(*cmd)->cmd = ft_strdup(" ");
	}
	(*cmd)->built_in = is_built_in((*cmd)->cmd);
	*head = add_cmd(*head, *cmd);
	if (*tmp && ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0)
	{
		(*tmp) = (*tmp)->prev;
		(*cmd)->has_pipe = 1;
		return ;
	}
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
	if (*tmp && (ft_strncmp((*tmp)->type, WORD, 4) == 0
			|| ft_strncmp((*tmp)->type, ENV, 1) == 0
			|| ft_strncmp((*tmp)->type, SPECIAL_CASE, 12) == 0))
		(*cmd)->cmd = (*tmp)->value;
	else
		(*cmd)->cmd = NULL;
	finish_cmd(cmd, tmp, head);
}

void	start_converting(t_token **tmp, t_cmd **head, t_cmd **cmd)
{
	int	found_cmd;

	found_cmd = 0;
	if (*tmp)
	{
		if (ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0
			&& (*tmp)->state != IN_DQUOTE && (*tmp)->state != IN_QUOTE)
			return ;
		if ((ft_strncmp((*tmp)->type, WORD, 4) == 0
				|| ft_strncmp((*tmp)->type, ENV, 1) == 0
				|| ft_strncmp((*tmp)->type, SPECIAL_CASE, 1) == 0)
			&& !found_cmd)
			convert_word(tmp, head, cmd, &found_cmd);
		else if (cmd_starts_with_redir(*tmp) && !found_cmd)
			convert_redirection(tmp, head, cmd, &found_cmd);
	}
}

int	convert_tokens_to_commands(t_data *data)
{
	t_token	*tmp;
	t_cmd	*head;
	t_cmd	*cmd;

	if (!data->token)
		return (0);
	tmp = data->token;
	head = NULL;
	cmd = NULL;
	tmp = clean_tokens(data, tmp);
	tmp = skip_white_spaces(tmp);
	if (!tmp || !check_syntax_error(tmp, data))
		return (0);
	while (tmp)
	{
		cmd = NULL;
		start_converting(&tmp, &head, &cmd);
		if (tmp)
			tmp = tmp->next;
		if (tmp && (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
			&& tmp->state == GENERAL)
			tmp = tmp->next;
	}
	data->cmd = head;
	return (1);
}
