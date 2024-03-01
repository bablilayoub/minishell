/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:50:50 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 16:08:45 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	is_built_in(char *value)
{
	if (ft_strncmp(value, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(value, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(value, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(value, "export", 7) == 0)
		return (true);
	if (ft_strncmp(value, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(value, "env", 4) == 0)
		return (true);
	if (ft_strncmp(value, "exit", 5) == 0)
		return (true);
	return (false);
}

t_cmd	*new_cmd(t_token *token)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->cmd = token->value;
	cmd->path = NULL;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->has_pipe = 0;
	cmd->output_files = NULL;
	cmd->input_files = NULL;
	cmd->arguments = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->built_in = is_built_in(token->value);
	return (cmd);
}

t_cmd	*add_cmd(t_cmd *head, t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = head;
	if (!head)
		return (cmd);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
	cmd->prev = tmp;
	return (head);
}

t_token	*add_file(char ***files, t_token *token)
{
	int		i;
	char	**new_files;

	if (!token)
		return (NULL);
	while (token && not_a_shell_command(token)
		&& ft_strncmp(token->type, WORD, 4) != 0)
		token = token->next;
	i = 0;
	while ((*files) && (*files)[i])
		i++;
	new_files = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while ((*files) && (*files)[i])
	{
		new_files[i] = ft_strdup((*files)[i]);
		free((*files)[i]);
		i++;
	}
	new_files[i] = ft_strdup(token->value);
	new_files[i + 1] = NULL;
	if (*files)
		free(*files);
	*files = new_files;
	return (token->next);
}
