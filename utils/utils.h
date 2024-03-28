/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 18:01:22 by abablil           #+#    #+#             */
/*   Updated: 2024/03/28 00:20:05 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../minishell.h"

bool	only_spaces(char *str, t_data *data);
char	**allocate_env(char **env, t_data *data);
char	**allocate_export(char **env, t_data *data);
bool	between_dquotes(t_token *token);
void	get_parent_pid(t_data *data);
char	**start_allocate_export(char **env, t_data *data, int *flag);
void	increment_shell_lvl(char **env, int *i, t_data *data);

#endif