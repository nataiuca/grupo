/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:14:57 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/27 11:51:08 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	args_split_len(t_tokens *curr)
{
	int	count;

	count = 0;
	while (curr && curr->type != pipe_type)
	{
		curr = skip_redir(curr);
		if (!curr || curr->type == pipe_type)
			break ;
		if (curr->type == command || curr->type == argument)
			count++;
		curr = curr->next;
	}
	return (count);
}

bool	extract_args(t_tokens *curr, char **args)
{
	int	i;

	i = 0;
	while (curr && curr->type != pipe_type)
	{
		if (!curr->was_quoted && curr->from_expansion
			&& is_empty_string(curr->content))
			curr = curr->next;
		curr = skip_redir(curr);
		if (!curr || curr->type == pipe_type)
			break ;
		if (curr->type == command || curr->type == argument)
		{
			args[i] = ft_strdup(curr->content);
			if (!args[i])
			{
				while (i-- > 0)
					free(args[i]);
				return (free(args), false);
			}
			i++;
		}
		curr = curr->next;
	}
	return (args[i] = NULL, true);
}

char	**split_args(t_tokens *tokens, t_program *program)
{
	t_tokens	*cmd_token;
	char		**args;
	int			count;

	cmd_token = skip_redir(tokens);
	if (!cmd_token || cmd_token->type != command)
	{
		args = malloc(sizeof(char *) * 2);
		if (!args)
		{
			ft_error(program, MSG_ERR_MALLOC, NULL, 1);
			return (NULL);
		}
		args[0] = ft_strdup(":");
		args[1] = NULL;
		return (args);
	}
	count = args_split_len(cmd_token);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args || !extract_args(cmd_token, args))
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	return (args);
}

bool	update_args(t_exec *exec, t_tokens *tokens, t_program *program)
{
	if (exec->args)
	{
		free_split_strs(exec->args);
	}
	exec->args = split_args(tokens, program);
	if (!exec->args || !exec->args[0])
	{
		return (false);
	}
	return (true);
}
