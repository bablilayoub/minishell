/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfrida <lfrida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:19:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/17 22:16:14 by lfrida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../utils/libft/libft.h"
#include "../minishell.h"
#include "../parsing/parser.h"

void initialize_path(t_cmd *head);
void executer(t_data *data);
int args_lenght(t_arg *args);
#endif