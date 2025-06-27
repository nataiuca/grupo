/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:06:51 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/27 01:54:40 by natferna         ###   ########.fr       */
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
// src/parcer/tokenizer.c

bool tokenizer(t_all *all, t_program *program)
{
	// Añade esto al inicio para ver la línea cruda:
    fprintf(stderr, "DEBUG (tokenizer): Raw input line: \"%s\"\n", all->line);
    if (is_quotes_present(all->line) && quotes_is_open(all->line))
        return (ft_error(program, ERR_OPEN_QUOTES, NULL, 1), false);

    check_spaces_line(&all->line, all->meta, program);

    if (is_quotes_present(all->line))
        all->token = ft_split_with_quotes(all->line, all->meta, program);
    else
        all->token = ft_split(all->line, ' ');
	// Añade esto para ver los tokens después del split:
    all->token = ft_split(all->line, ' ');
    fprintf(stderr, "DEBUG (tokenizer): Tokens after split:\n");
    for (int i = 0; all->token && all->token[i]; i++)
        fprintf(stderr, "  [%d]: \"%s\"\n", i, all->token[i]);

    if (!all->token)
        return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), false);

    init_list(&all->tokens, all->token, program);
    free_split_strs(all->token);
    all->token = NULL;

    check_type_tokens(all->tokens, all->meta);

    if (!check_token_has_var(all->tokens, all->meta, program))
        return (false);

    check_and_quit_quotes_token(all->tokens, all->meta);

    // CORRECCIÓN AQUÍ: Añadir 'all' como tercer argumento
    if (!check_bash_pipe_error(all->tokens, program, all)) // <-- AÑADIDO 'all'
        return (false);

    return (true);
}

void handle_reset(t_all *all, t_program *program)
{
	(void)program;
    free_all_structs(all); // Libera todas las estructuras de 'all'
    // all->line ya no se usa para pedir la línea aquí.
    // El bucle principal se encargará de pedir la siguiente línea.
    // Asegurarse de que all.line sea NULL si se usó para evitar dobles frees
}
