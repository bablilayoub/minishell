/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/26 01:29:44 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int not_a_shell_command(t_token *token)
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
				printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", tmp->args->arg, 8 - (int)ft_strlen(tmp->args->arg), " ", tmp->args->env_var);
				tmp->args = tmp->args->next;
			}
			tmp->args = arg;
		}
		if (tmp->redirect_in)
			printf("| Red In   : '%s' %*s |\n", tmp->redirect_in, 26 - (int)ft_strlen(tmp->redirect_in), " ");
		else
			printf("| Red In   : '%s' %*s |\n", "NULL", 26 - 4, " ");
		if (tmp->input_file)
			printf("| In File  : '%s' %*s |\n", tmp->input_file, 26 - (int)ft_strlen(tmp->input_file), " ");
		else
			printf("| In File  : '%s' %*s |\n", "NULL", 26 - 4, " ");
		if (tmp->redirect_out)
			printf("| Red Out  : '%s' %*s |\n", tmp->redirect_out, 26 - (int)ft_strlen(tmp->redirect_out), " ");
		else
			printf("| Red Out  : '%s' %*s |\n", "NULL", 26 - 4, " ");
		if (tmp->output_file)
			printf("| Out File : '%s' %*s |\n", tmp->output_file, 26 - (int)ft_strlen(tmp->output_file), " ");
		else
			printf("| Out File : '%s' %*s |\n", "NULL", 26 - 4, " ");
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

void convert_tokens_to_commands(t_data *data)
{
	if (!data->token)
		return;
	t_token *tmp = data->token;
	t_cmd *head = NULL;
	int found_cmd = 0;

	while (tmp)
	{
		t_cmd *cmd = NULL;
		found_cmd = 0;
		while (tmp)
		{
			if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE)
				break;
			if (ft_strncmp(tmp->type, WORD, 4) == 0 && !found_cmd)
			{
				found_cmd = 1;
				cmd = new_cmd(tmp);
				head = add_cmd(head, cmd);
				if (tmp->next)
					tmp = find_args(cmd, tmp->next);
				else
					cmd->args = add_arg(cmd->args, cmd->cmd, 1);
				break;
			}
			tmp = tmp->next;
			if (tmp)
				tmp = skip_white_spaces(tmp);
		}
		if (tmp)
			tmp = tmp->next;
		if (tmp && (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0))
			tmp = tmp->next;
	}
	data->cmd = head;
	print_args(data->cmd);
}

char *get_env(char *env, t_data *data)
{
	int i;
	int j;
	char *env_var;
	char *env_value;

	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] && data->env[i][j] != '=')
			j++;
		env_var = ft_substr(data->env[i], 0, j);
		if (ft_strncmp(env_var, env, ft_strlen(env_var)) == 0)
		{
			env_value = ft_strdup(data->env[i] + j + 1);
			free(env_var);
			return (env_value);
		}
		free(env_var);
		i++;
	}
	return (NULL);
}

void get_env_vars(t_data *data)
{
	t_cmd *tmp = data->cmd;
	t_arg *arg;
	char *env_var;
	char *exit_status;

	while (tmp)
	{
		arg = tmp->args;
		while (tmp->args)
		{
			if (ft_strncmp(tmp->args->arg, "$", 1) == 0 && tmp->args->env_var == 1)
			{
				if (ft_strncmp(tmp->args->arg, "$?", 2) == 0)
				{
					exit_status = ft_itoa(data->exit_status);
					tmp->args->arg = ft_strjoin(exit_status, tmp->args->arg + 2);
				}
				else
				{
					env_var = get_env(tmp->args->arg + 1, data);
					if (!env_var)
						tmp->args->arg = ft_strdup("");
					else
						tmp->args->arg = ft_strdup(env_var);
				}
			}
			tmp->args = tmp->args->next;
		}
		tmp->args = arg;
		tmp = tmp->next;
	}
}