/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:12:06 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 21:50:56 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*find_cmd (t_tokens *tokens)
{
	t_tokens	*curr;

	curr = tokens;
	while (curr)
	{
		curr = skip_redir(curr);
		if (!curr)
			return (NULL);
		if (curr->type == command)
		{
			return (curr);
		}
		curr = curr->next;
	}
	return (NULL);
}

t_tokens	*skip_redir(t_tokens *curr)
{
	while (curr && (curr->type == redir_r_u || curr->type == redir_r_d
			|| curr->type == redir_l_u || curr->type == redir_l_d))
	{
		curr = curr->next;
		if (curr)
			curr = curr->next;
	}
	return (curr);
}

//✅ para revisar este caso: ""  y '' debe ser como -> Command '' not found. en f() (t_all all, t_program *program)

bool	is_empty_string(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (true);
	while (s[i])
	{
		if (!ft_isspace(s[i]) && s[i] != '\'' && s[i] != '\"')
			return (false);
		i++;
	}
	return (true);
}
	

bool	is_empty_cmd(char *s)
{

	if (!s)
		return (true);
	if (s[0] == '\0')
		return (true);
	return (false);
}

int	count_type_tokens(t_tokens *tokens, t_type type_to_find)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == type_to_find)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

bool init_exec_vals(t_program *program, t_all *all)
{
    // ELIMINAR ESTA LÍNEA:
    // all->here->base_cwd = ft_strdup(program->prompt_cwd);

    // Asegurarse de que all->here->base_cwd ya esté inicializado
    // (ahora se hace en init_here_struct)
    if (!all->here->base_cwd) // Esto debería ser una comprobación de seguridad, no el punto de asignación
    {
        ft_error(program, "Internal error: base_cwd not initialized in init_here_struct", NULL, 1);
        return (false);
    }

    all->exec->num_pipes = count_type_tokens(all->tokens, pipe_type);
    all->exec->num_cmds = count_type_tokens(all->tokens, command);
    return (true);
}
