/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:06:51 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/07 10:25:21 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Performs the tokenization process, splitting the input line
 * into a list of tokens and validating their structure.
 *
 * @param all A pointer to the main `t_all` structure containing the
 * input line and where tokens will be stored.
 * @param program A pointer to the `t_program` for error handling and
 * shell state.
 * @return True if tokenization is successful and syntax is valid, false
 * otherwise.
 */
bool	tokenizer(t_all *all, t_program *program)
{
	if (is_quotes_present(all->line) && quotes_is_open(all->line))
		return (ft_error(program, ERR_OPEN_QUOTES, NULL, 1), false);
	check_spaces_line(&all->line, all->meta, program);
	if (is_quotes_present(all->line))
		all->token = ft_split_with_quotes(all->line, all->meta, program);
	else
		all->token = ft_split(all->line, ' ');
	if (!all->token)
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), false);
	init_list(&all->tokens, all->token, program);
	free_split_strs(all->token);
	all->token = NULL;
	check_type_tokens(all->tokens, all->meta);
	if (!check_token_has_var(all->tokens, all->meta, program))
		return (false);
	check_and_quit_quotes_token(all->tokens, all->meta);
	if (!check_bash_pipe_error(all->tokens, program))
		return (false);
	return (true);
}

void	handle_reset(t_all *all, t_program *program)
{
	free_all_structs(all, 0);
	all->line = readline(get_prompt(program));
	program->input_status = INPUT_ERROR;
}
