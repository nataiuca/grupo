/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:42:51 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/07 08:51:51 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

//#➵⤐──╌╌➣⋆➣── Includes minishell ──➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#

# include "datatypes.h"

//#➵⤐──╌╌➣⋆➣── Define_error_tokenizer ─➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌➔#

# define ERR_OPEN_QUOTES "minishell_error: open quotes."
# define ERR_OPEN_PIPE "minishell_error: open pipe.\n"
# define ERR_VAR "Error: The alloc size for vars expansions has been exceeded: "
# define ERR_HERE_DOC_SIZE "Error: The allow num of here-doc has been exceeded."
# define ERR_GETCWD "getcwd error, check your actual dir... returning to home"
# define ERR_BASH "bash: syntax error near unexpected token"

//#➵⤐──╌╌➣⋆➣── Parse ─────────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/parcer/minishell_loop.c
// static bool	handle_atomic_interrupt(t_program *program);
// static bool	handle_empty_input(t_program *program, t_all *all);
// static bool	parse_and_execute(t_program *program, t_all *all);
// static bool	reset_and_check_exit(t_program *program, t_all *all);
void		minishell_loop(t_program *program);

// src/parcer/tokenizer.c
bool		tokenizer(t_all *all, t_program *program);
void		handle_reset(t_all *all, t_program *program);

// src/parcer/handle_error.c
void		write_error_message(const char *msg, const char *arg);
void		ft_error(t_program *program, const char *msg, const char *arg,
				int exit_st);

// src/parcer/tokenizer_utils.c
int			get_or_set_last_exit_status(int new_status, bool set);
t_tokens	*ft_find_node_n_position(t_tokens *head, int position);
bool		ft_str_is_empty_or_spaces(const char *str);
void		reset_meta_flags(t_metachars *meta, int flag);

// src/parcer/get_prompt.c
char		*get_hostname(void);
void		build_shell_name(char *dest);
void		build_colored_parts(t_program *program, char *temp_cwd,
				char *colored_userhost, char *colored_path);
char		*get_prompt(t_program *program);

// src/parcer/check_spaces_line_utils.c (5/5)
bool		needs_space_before(const char *line, int i);
bool		needs_space_after(const char *line, int i, int op_len);
int			count_extra_spaces(const char *line, t_metachars *meta);
int			count_spaces_for_ops(const char *line, t_metachars *meta);
char		*allocate_new_line(const char *line, t_metachars *metav,
				t_program *program);

// src/parcer/check_spaces_line.c
void		copy_operator(char *new_line, t_metachars *meta, const char *line,
				int op_len);
void		process_operators(const char *line, char *new_line,
				t_metachars *meta);
void		process_line(const char *line, char *new_line, t_metachars *meta);
void		replace_line(char **line, char *new_line, t_program *program);
void		check_spaces_line(char **line, t_metachars *meta,
				t_program *program);

// src/parcer/ckeck_quote_utils.c
bool		is_quotes_present(const char *str);
bool		quotes_is_open(const char *str);
void		toggle_quotes(char c, bool *in_single_quote, bool *in_double_quote);
void		update_quote_status(const char c, t_metachars *meta);
bool		is_operator(const char *str, const char *op);

// src/parcer/ft_split_with_quotes.c
int			count_tokens(const char *sts, t_metachars *meta);
char		*extract_token(const char *sts, t_metachars *meta,
				t_program *program);
char		**ft_split_with_quotes(char const *sts, t_metachars *meta,
				t_program *program);

// src/parcer/quit_quotes_token.c
bool		is_outer_quote(char *content, int i, bool *in_single_quote,
				bool *in_double_quote);
bool		is_inner_quote(char *content, int i);
void		quit_quotes_token(t_tokens *current, t_metachars *meta);
void		check_and_quit_quotes_token(t_tokens *tokens, t_metachars *meta);

// src/parcer/tokens_type_check_type.c
bool		try_set_as_redirection(t_tokens *current, t_tokens *head,
				t_metachars *meta);
bool		try_set_as_redir_follower(t_tokens *current, t_tokens *head,
				t_metachars *meta);
bool		assign_token_type(t_tokens *current, t_tokens *head,
				t_metachars *meta);
void		process_tokens(t_tokens *tokens, bool *has_temp_tokens,
				t_metachars *meta);
void		check_type_tokens(t_tokens *tokens, t_metachars *meta);

// src/parcer/tokens_type_command_check.c
bool		is_command_after_pipe(t_tokens *current, t_tokens *prev);
bool		is_first_command(t_tokens *current);
bool		check_command(t_tokens *current, t_tokens *head, t_metachars *meta);
bool		try_set_as_command(t_tokens *current, t_tokens *head,
				t_metachars *meta);
bool		try_set_as_argument(t_tokens *current, t_tokens *head,
				t_metachars *meta);

// src/parcer/tokens_type_redir_check.c
bool		is_redir(t_tokens *current);
t_type		check_redir(t_tokens *current);
bool		invalid_after_redir(t_tokens *current, t_tokens *head,
				t_type expected_redir);
void		assign_redirection_type(t_tokens *current, t_tokens *head);

// src/parcer/tokens_type_pipe_check.c
bool		try_set_as_pipe(t_tokens *current, t_tokens *next_token,
				t_metachars *meta);
bool		check_pipe(t_tokens *current);

// src/parcer/syntax_error_token_utils.c
bool		is_problematic_redir_pattern(t_tokens *tokens);
bool		is_pipe_heredoc_at_start(t_tokens *tokens);
bool		check_bash_before_here(t_tokens *tokens);

// src/parcer/syntax_error_token.c
bool		check_bash_pipe_before_here(t_tokens *tokens);
bool		check_open_pipe_token(t_tokens *tokens);
bool		check_bash_error_token(t_tokens *tokens);
bool		check_bash_after_here(t_tokens *tokens);
bool		check_bash_pipe_error(t_tokens *tokens, t_program *program);

// src/parcer/var_lookup.c
int			extract_var_name(const char *str, char *var_name);
char		*expand_var(t_program *program, char *var_name);

// src/parcer/var_expand_core.c
char		*process_character(const char *original, t_metachars *meta,
				char *new_str, int *new_size);
void		handle_expansion(const char *original, t_metachars *meta,
				char **new_str, int *new_size);
bool		append_var_value_to_str(char **new_str, int *new_size,
				char *var_value, t_metachars *meta);
char		*get_valid_var_value(t_program *program, const char *original,
				t_metachars *meta, int *len);
char		*update_string_size(char *new_str, int *new_size,
				int min_extra_size);

// src/parcer/var_check_expand.c
char		*init_new_str(t_program *program, int *size);
bool		has_space_for_null_terminator(t_program *program, t_metachars *meta,
				int size);
char		*build_new_content(const char *original, t_metachars *meta,
				t_program *program);
bool		check_and_expand_var(t_tokens *tokens, char **str,
				t_metachars *meta, t_program *program);
bool		check_token_has_var(t_tokens *tokens, t_metachars *meta,
				t_program *program);

#endif