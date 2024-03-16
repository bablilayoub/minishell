/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:45:48 by abablil           #+#    #+#             */
/*   Updated: 2024/03/16 02:37:52 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	is_special_char_token(t_token *tmp)
{
	return ((ft_strncmp(tmp->type, REDIR_IN, 1) == 0
			|| ft_strncmp(tmp->type, REDIR_OUT, 1) == 0
			|| ft_strncmp(tmp->type, APPEND_OUT, 2) == 0
			|| ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
		&& (tmp->state == IN_QUOTE || tmp->state == IN_DQUOTE));
}

int	handle_escape_dquote_end_error(t_token **tmp)
{
	if (ft_strncmp((*tmp)->type, ESCAPE, 1) == 0
		&& ((*tmp)->state == IN_DQUOTE) && !(*tmp)->next)
	{
		printf(PREFIX_ERROR "Syntax error\n");
		return (0);
	}
	return (1);
}

int	handle_more_tokens(t_token **tmp, t_arg **head)
{
	if (ft_strncmp((*tmp)->type, QUOTE, 1) == 0
		&& between_dquotes(*tmp) && ((*tmp)->state == IN_DQUOTE))
		*head = add_arg(*head, (*tmp)->value, 1);
	else if (ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0
		&& ((*tmp)->state == IN_QUOTE || (*tmp)->state == IN_DQUOTE))
		*head = add_arg(*head, (*tmp)->value, 1);
	else if ((ft_strncmp((*tmp)->type, NEW_LINE, 1) == 0
			|| ft_strncmp((*tmp)->type, TAB_SPACE, 1) == 0)
		&& ((*tmp)->state == IN_QUOTE || (*tmp)->state == IN_DQUOTE))
		*head = add_arg(*head, (*tmp)->value, 1);
	else if (ft_strncmp((*tmp)->type, ESCAPE, 1) == 0
		&& ((*tmp)->state == IN_QUOTE))
		*head = add_arg(*head, (*tmp)->value, 1);
	else if (!handle_escape_dquote_end_error(tmp))
		return (0);
	return (1);
}

int	start_finding_args(t_token **tmp, t_cmd **cmd,
	t_arg **head, char *tmp_type)
{
	while (*tmp)
	{
		if (should_stop_finding_args(*tmp))
			break ;
		if (!handle_tokens(tmp, head, cmd, tmp_type))
		{
			if (is_special_char_token(*tmp))
				*head = add_arg(*head, (*tmp)->value, 1);
			else if (is_white_space_in_quote(*tmp))
				*head = add_arg(*head, (*tmp)->value, 1);
			else if (ft_strncmp((*tmp)->type, WHITE_SPACE, 1) == 0
				&& ((*tmp)->state == GENERAL))
			{
				handle_general_white_space(tmp, head, cmd);
				continue ;
			}
			else if (!handle_more_tokens(tmp, head))
				return (0);
		}
		if (*tmp && (*tmp)->next)
			*tmp = (*tmp)->next;
		else
			break ;
	}
	return (1);
}

t_token	*find_args(t_cmd *cmd, t_token *token)
{
	t_token	*tmp;
	t_arg	*head;
	char	*tmp_type;

	if (!token || !cmd)
		return (NULL);
	tmp = token;
	head = NULL;
	head = add_arg(head, cmd->cmd, 1);
	tmp = skip_white_spaces(tmp);
	tmp_type = NULL;
	if (!start_finding_args(&tmp, &cmd, &head, tmp_type))
		return (NULL);
	if (tmp && ft_strncmp(tmp->type, PIPE_LINE, 1) == 0
		&& tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE)
		cmd->has_pipe = 1;
	cmd->args = head;
	return (tmp);
}
