/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:19:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/23 18:05:03 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../utils/libft/libft.h"
#include "../minishell.h"
#include "../parsing/parser.h"

#define CLEAR "\033[H\033[2J"


bool initialize_path(t_cmd *head);
int args_lenght(t_arg *args);
int cmd_lenght(t_cmd *cmd);
void close_fds(int fd[][2], int cmd);
t_cmd *parsed_list();
void prepare_for_excution(t_data *data);
void initialize_arguments(t_cmd *cmd_list);
void ft_echo(t_cmd *cmd);
int ft_chdir(t_cmd *cmd);
void ft_pwd();
void ft_export(t_cmd *cmd, char ***envp);
void ft_env(char **env);
void ft_exit(t_cmd *cmd);
void excute_builtin(t_cmd *cmd_list, t_data *data);
#endif