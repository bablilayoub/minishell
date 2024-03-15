/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:45:48 by abablil           #+#    #+#             */
/*   Updated: 2024/03/15 17:44:52 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

// int	should_stop_finding_args(t_token *tmp)
// {
// 	return (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0
// 		&& tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE);
// }

int	start_finding_args(t_token **tmp, t_cmd **cmd,
	t_arg **head, char *tmp_type)
{
	while (*tmp)
	{
		if (ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0
			&& (*tmp)->state != IN_DQUOTE && (*tmp)->state != IN_QUOTE)
			break ;
		if (ft_strncmp((*tmp)->type, WORD, 4) == 0)
			*head = add_arg(*head, (*tmp)->value, 1);
		else if (ft_strncmp((*tmp)->type, ENV, 1) == 0)
			*head = add_arg(*head, (*tmp)->value, ((*tmp)->state == IN_QUOTE));
		else if ((ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0
				|| ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0)
			&& ((*tmp)->state != IN_QUOTE && (*tmp)->state != IN_DQUOTE))
		{
			tmp_type = (*tmp)->type;
			if ((*tmp)->next)
				*tmp = skip_white_spaces((*tmp)->next);
			if (tmp && ft_strncmp((*tmp)->type, WORD, 4) == 0)
			{
				if (ft_strncmp(tmp_type, APPEND_OUT, 2) == 0)
					*tmp = add_file(cmd, &(*cmd)->redirect_out,
							*tmp, APPEND_OUT);
				else if (ft_strncmp(tmp_type, REDIR_OUT, 1) == 0)
					*tmp = add_file(cmd, &(*cmd)->redirect_out,
							*tmp, REDIR_OUT);
			}
			else
				*tmp = add_file(cmd, &(*cmd)->redirect_out, NULL, (*tmp)->type);
		}
		else if ((ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)
			&& ((*tmp)->state != IN_QUOTE && (*tmp)->state != IN_DQUOTE))
		{
			tmp_type = (*tmp)->type;
			if ((*tmp)->next)
				*tmp = skip_white_spaces((*tmp)->next);
			if (tmp && ft_strncmp((*tmp)->type, WORD, 4) == 0)
			{
				if (ft_strncmp(tmp_type, HERE_DOC, 2) == 0
					|| ft_strncmp(tmp_type, REDIR_IN, 1) == 0)
					*tmp = add_file(cmd, &(*cmd)->redirect_in,
							*tmp, (*tmp)->type);
			}
			else
				*tmp = add_file(cmd, &(*cmd)->redirect_in, NULL, (*tmp)->type);
		}
		else if ((ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0
				|| ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0
				|| ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0
				|| ft_strncmp((*tmp)->type, HERE_DOC, 2) == 0)
			&& ((*tmp)->state == IN_QUOTE || (*tmp)->state == IN_DQUOTE))
			*head = add_arg(*head, (*tmp)->value, 1);
		else if (ft_strncmp((*tmp)->type, WHITE_SPACE, 1) == 0
			&& ((*tmp)->state != GENERAL))
			*head = add_arg(*head, (*tmp)->value, 1);
		else if (ft_strncmp((*tmp)->type, WHITE_SPACE, 1) == 0
			&& ((*tmp)->state == GENERAL))
		{
			*tmp = skip_white_spaces(*tmp);
			if (tmp && (ft_strncmp((*cmd)->cmd, "echo", 4) == 0)
				&& not_a_shell_command(*tmp))
				*head = add_arg(*head, " ", 1);
			continue ;
		}
		else if (ft_strncmp((*tmp)->type, QUOTE, 1) == 0
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
		else if (ft_strncmp((*tmp)->type, ESCAPE, 1) == 0
			&& ((*tmp)->state == IN_DQUOTE) && !(*tmp)->next)
		{
			printf(PREFIX_ERROR "Syntax error\n");
			return (0);
		}
		if (tmp && (*tmp)->next)
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
