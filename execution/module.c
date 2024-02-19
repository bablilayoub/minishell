/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:14:34 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/19 15:58:49 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_cmd *create_cmd(char *cmd, char *path, char *redirect, char *output_file, char **arguments)
{
    t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (new_cmd == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_cmd->cmd = cmd;
    new_cmd->path = path;
    new_cmd->redirect = redirect;
    new_cmd->output_file = output_file;
    new_cmd->arguments = arguments;
    new_cmd->next = NULL;
    return new_cmd;
}

void append_cmd(t_cmd **head, char *cmd, char *path, char *redirect, char *output_file, char **arguments)
{
    t_cmd *new_cmd = create_cmd(cmd, path, redirect, output_file, arguments);
    if (*head == NULL)
    {
        *head = new_cmd;
        return;
    }
    t_cmd *current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_cmd;
}

t_cmd *parsed_list()
{

    t_cmd *cmd_list = NULL;

    char **argv1 = malloc(sizeof(char *) * 3);
    char **argv2 = malloc(sizeof(char *) * 3);
    char **argv3 = malloc(sizeof(char *) * 2);
    char **argv4 = malloc(sizeof(char *) * 4);

    argv1[0] = strdup("ls");
    argv1[1] = strdup("-l");
    argv1[2] = NULL;

    argv2[0] = strdup("grep");
    argv2[1] = strdup("utils.c");
    argv2[2] = NULL;

    argv3[0] = strdup("wc");
    argv3[1] = NULL;

    
    argv4[0] = strdup("grep");
    argv4[1] = strdup("-n");
    argv4[2] = strdup("9");
    argv4[3] = NULL;

    append_cmd(&cmd_list, "ls", "/bin/ls", NULL, NULL, argv1);
    append_cmd(&cmd_list, "grep", "/usr/bin/grep", NULL, NULL, argv2);
    append_cmd(&cmd_list, "wc", "/usr/bin/wc", NULL, NULL, argv3);
    append_cmd(&cmd_list, "grep", "/usr/bin/grep", NULL, NULL, argv4);
    return cmd_list;
}

