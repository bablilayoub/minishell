/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:19:02 by alaalalm          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/02/19 16:05:20 by alaalalm         ###   ########.fr       */
=======
/*   Updated: 2024/02/19 12:07:49 by alaalalm         ###   ########.fr       */
>>>>>>> Stashed changes
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

#define CLEAR "\033[H\033[2J"


void initialize_path(t_cmd *head);
int args_lenght(t_arg *args);
int cmd_lenght(t_cmd *cmd);
void close_fds(int fd[][2], int cmd);
t_cmd *parsed_list();
<<<<<<< Updated upstream
void prepare_for_excution(t_cmd *cmd_list);
=======
void prepare_for_excution();
>>>>>>> Stashed changes
#endif