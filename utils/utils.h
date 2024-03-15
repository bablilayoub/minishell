/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 18:01:22 by abablil           #+#    #+#             */
/*   Updated: 2024/03/15 02:59:38 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../minishell.h"

bool	only_spaces(char *str);
char	**allocate_env(char **env);
char	**allocate_export(char **env);
bool	between_dquotes(t_token *token);

#endif