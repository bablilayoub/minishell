/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/03/20 00:33:22 by alaalalm         ###   ########.fr       */
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

void print_args(t_cmd *head)
{
	t_cmd *tmp = head;
	t_arg *arg;

	if (!tmp)
		return;
	while (tmp)
	{
		printf("--------------------------------------------\n");
		printf("| Command  : %s %*s |\n", tmp->cmd, 28 - (int)ft_strlen(tmp->cmd), " ");
		if (tmp->args)
		{
			arg = tmp->args;
			while (tmp->args)
			{
				if (!tmp->args->arg)
					break;
				if (ft_strncmp(tmp->args->arg, NEW_LINE, 1) != 0 && ft_strncmp(tmp->args->arg, TAB_SPACE, 1) != 0)
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", tmp->args->arg, 8 - (int)ft_strlen(tmp->args->arg), " ", tmp->args->env_var);
				else if (ft_strncmp(tmp->args->arg, NEW_LINE, 1) == 0)
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", "\\n", 8 - 2, " ", tmp->args->env_var);
				else
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", "\\t", 8 - 2, " ", tmp->args->env_var);
				tmp->args = tmp->args->next;
			}
			tmp->args = arg;
		}
		if (tmp->has_redirection)
		{
			t_redirection *redir = tmp->redirects;
			printf("|------------------------------------------|\n");
			printf("| Redirection %*s |\n", 34, " ");
			while (redir)
			{
				printf("| Type     : '%s' %*s |\n", redir->type, 26 - (int)ft_strlen(redir->type), " ");
				printf("| File     : '%s' %*s |\n", redir->file, 26 - (int)ft_strlen(redir->file), " ");
				if (redir->next)
					printf("|%*s|\n", 42, " ");
				redir = redir->next;
			}
			printf("|------------------------------------------|\n");
		}
		if (tmp->has_pipe)
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		else
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		if (tmp->built_in)
			printf("| Built In : %d %*s |\n", tmp->built_in, 27, " ");
		else
			printf("| Built In : %d %*s |\n", tmp->built_in, 27, " ");
		printf("--------------------------------------------\n");
		tmp = tmp->next;
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
	// print_tokens(tmp);
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
	// print_args(data->cmd);
}
