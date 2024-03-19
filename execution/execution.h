/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:19:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/19 21:36:52 by alaalalm         ###   ########.fr       */
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

void	check_error(int fd, const char *msg, int flag);
void	check_error_null(void *ptr, const char *msg);
bool	initialize_path(t_cmd *head, t_data *data);
int		args_lenght(t_arg *args);
int		cmd_lenght(t_cmd *cmd);
void	prepare_for_excution(t_data *data);
void	initialize_arguments(t_cmd *cmd_list);
void	close_fds(int **fd);
void	close_fds_and_getstatus(t_data *data);
char	**key_value(char *exported);
void	excute_builtin(t_cmd *cmd_list, t_data *data);
void	ft_echo(t_cmd *cmd);
void	ft_chdir(t_cmd *cmd, t_data *data);
void	ft_pwd(t_data *data);
void	ft_export(t_data *data, char ***env);
bool	ft_env(char **env, t_data *data);
void	ft_exit(t_data *data, t_cmd *cmd);
void	ft_unset(t_cmd *cmd, char ***env, char ***export);
void	update_prefix(t_data *data, char *prefix);
char	**update_envpwd(char **env, char *oldpwd, char *pwd);
int		ft_strdoublelen(char **str);
void	free_double(char **env);
void	free_triplet(char *s1, char *s2, char *s3);
void	free_two(char *s1, char *s2);
void	print(char **export);
char	*ft_getenv(char *name, char **env);
void	export_to_exp(char ***export, char **args, t_data *data);
int		check_exported(char *exported, int flag, t_data *data);
char	*ft_getenv(char *name, char **env);
void	print_error(t_cmd *cmd, char *dirname, char *oldpwd);
void	add_var(char ***env, char *exported, int flag);
void	handle_redirections(t_cmd *cmd, int **fd, int k, t_data *data);
int		handle_single_command_redirections(t_cmd *cmd, t_data *data);
int		ft_is_alphanumeric(char *key);
int		here_doc(char *file_and_search_for);
int		redirections_in(t_cmd *cmd, int *fd_in,
			t_redirection *tmp, t_data *data);
int		redirections_out(t_cmd *cmd, int *fd_out,
			t_redirection *tmp, t_data *data);
void	free_int(int **fd);
bool	failure(char *cmd, t_data *data);
int		get_file(t_cmd *cmd, char *file);

#endif