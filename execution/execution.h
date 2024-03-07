/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:19:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/07 02:33:29 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <errno.h>
# include <dirent.h>
# include <sys/wait.h>
# include <stdlib.h>
# include "../utils/libft/libft.h"
# include "../minishell.h"
# include "../parsing/parser.h"
# include <fcntl.h>

# define CLEAR "\033[H\033[2J"
void  check_error(int fd, const char *msg, int flag);
void  check_error_null(void *ptr, const char *msg, int flag);
bool  initialize_path(t_cmd *head, t_data *data);
int  args_lenght(t_arg *args);
int  cmd_lenght(t_cmd *cmd);
void  prepare_for_excution(t_data *data);
void  initialize_arguments(t_cmd *cmd_list);
void  close_fds(int fd[][2], int cmd);
void  close_fds_and_wait(int fd[][2], pid_t pid[], int fd_c, t_data *data);
char **key_value(char *exported, char **env);
// b uilt_in
void  excute_builtin(t_cmd *cmd_list, t_data *data);
void  ft_echo(t_cmd *cmd);
void  ft_chdir(t_cmd *cmd, t_data *data);
void  ft_pwd();
void ft_export(t_data *data, char **env);
void  ft_env(char **env);
void  ft_exit(t_data *data, t_cmd *cmd);
void  ft_unset(t_data *data, t_cmd *cmd, char **env);
// p arent incoming
void   update_prefix(t_data *data, char *prefix);
char  **update_envpwd(char **env, char *oldpwd, char *pwd, char *path);
void  change_path(t_data *data);
void  exit_shell(t_data *data);
void    update_env(t_data *data, char *cmd_name, char **args);
void  exit_shell(t_data *data);
int ft_strdoublelen(char **str);

// frees

void    free_double(char **env);
void    free_triplet(char *s1, char *s2, char *s3);
void    free_two(char *s1, char *s2);

void print(char **export);
char *ft_getenv(char *name, char **env);
void export_env_to_exp(char **env, char ***export);
void join_double(char ***join, char **tmp);
#endif