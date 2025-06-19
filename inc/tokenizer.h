/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:42:51 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/18 11:42:10 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes minishell:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "datatypes.h"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Macros_error_tokenizer:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

//	**ERROR MESSAGES: MINISHELL**
# define ERR_OPEN_QUOTES		"minishell_error: open quotes."
# define ERR_OPEN_PIPE			"minishell_error: open pipe.\n"
# define ERR_VAR				"Error: The allocated size for variable expansions has been exceeded: "// getconf ARG_MAX -> 2.097.152 bytes (42)
# define ERR_HERE_DOC_SIZE		"Error: The allowed number of here-docs has been exceeded."   
# define ERR_GETCWD				"getcwd error, check your actual directory... returning to home"
# define ERR_BASH				"bash: syntax error near unexpected token"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Prototypes:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

//➛ src/parcer/minishell_loop.c (/5) ⚠️_NORMA + ⚠️_DESCRIPCIONES c + ✅_se_usa
void				minishell_loop(t_program *program);


// src/parcer/tokenizer.c (/5) ✅_NORMA + ✅_DESCRIPCIONES + ⚠️_testeo + ✅_se_usa
bool				tokenizer(t_all *all, t_program *program);
bool				handle_reset(t_all *all,  t_program *program);


// src/parcer/handle_error.c (/5)	// ⚠️_NORMA + ⚠️_testeo  + ⚠️_DESCRIPCIONES + ✅_se_usa
void				write_error_message(const char *msg, const char *arg);
void				ft_error(t_program *program, const char *msg, const char *arg, int exit_st);


// src/parcer/tokenizer_utils.c (/5)	// ⚠️_NORMA + ⚠️_testeo  + ⚠️_DESCRIPCIONES + 🚩 revisar_SI SE USA
int					get_or_set_last_exit_status(int new_status, bool set);
//char				*get_line(void);									// 🚩 revisar
char				*get_line_prompt(t_program *program);				// 🚩 solo se usa en dev en vez de readline
char				*get_line_str(char *str, t_program *program);							//✅_se usa en here_doc
t_tokens			*ft_find_node_n_position(t_tokens *head, int position);
bool				ft_str_is_empty_or_spaces(const char *str); 	//✅_se_usa
void				reset_meta_flags(t_metachars *meta, int flag);	//✅_se_usa


// src/parcer/get_prompt.c (4/5) ✅_NORMA + ✅_DESCRIPCIONES  + ⚠️_testeo + ✅_se_usa
char				*get_hostname(void);
void				build_shell_name(char *dest);
void				build_colored_parts(t_program *program, char *temp_cwd, char *colored_userhost, char *colored_path);
void				getcwd_error(t_program *program);
char				*get_prompt(t_program *program);	


//--------------check_spaces_line──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// src/parcer/check_spaces_line_utils.c (5/5)  ✅__NORMA + ✅_DESCRIPCIONES + ⚠️_testeo + ✅_se_usa
bool				needs_space_before(const char *line, int i);
bool				needs_space_after(const char *line, int i, int op_len);
int					count_extra_spaces(const char *line, t_metachars *meta);
int					count_spaces_for_ops(const char *line, t_metachars *meta);
char				*allocate_new_line(const char *line, t_metachars *metav, t_program *program);

// src/parcer/check_spaces_line.c (5/5) ✅__NORMA + ✅_DESCRIPCIONES  + ⚠️_testeo + ✅_se_usa
void				copy_operator(char *new_line, t_metachars *meta, const char *line, int op_len);
void				process_operators(const char *line, char *new_line, t_metachars *meta);
void				process_line(const char *line, char *new_line, t_metachars *meta);
void				replace_line(char **line, char *new_line, t_program *program);
void				check_spaces_line(char **line, t_metachars *meta, t_program *program);
//-----------check_spaces_line_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


//------quotes─╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔# 
// src/parcer/ckeck_quote_utils.c (/5):  ✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES  + ✅_se_usa
bool				is_quotes_present(const char *str);
bool				quotes_is_open(const char *str);
void				toggle_quotes(char c, bool *in_single_quote, bool *in_double_quote);
void				update_quote_status(const char c, t_metachars *meta);
bool				is_operator(const char *str, const char *op);

// src/parcer/ft_split_with_quotes.c (3/5)	✅__NORMA + ⚠️_testeo  + ✅_DESCRIPCIONES + ✅_se_usa
int					count_tokens(const char *sts, t_metachars *meta);
char				*extract_token(const char *sts, t_metachars *meta, t_program *program);		
char				**ft_split_with_quotes(char const *sts, t_metachars *meta, t_program *program);

// src/parcer/quit_quotes_token.c  (4/5)	✅__NORMA  + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				is_outer_quote(char *content, int i, bool *in_single_quote, bool *in_double_quote);
bool				is_inner_quote(char *content, int i);
void				quit_quotes_token(t_tokens *current, t_metachars *meta);
void				check_and_quit_quotes_token(t_tokens *tokens, t_metachars *meta);
//------quotes end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


//--check_type_tokens──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// src/parcer/tokens_type_check_type.c (/5)  ✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				try_set_as_redirection(t_tokens *current, t_tokens *head);
bool				try_set_as_argument_if_unused(t_tokens *current, t_tokens *head);
bool				assign_token_type(t_tokens *current, t_tokens *head);
void				process_tokens(t_tokens *tokens, bool *has_temp_tokens);
void				check_type_tokens(t_tokens *tokens);

// src/parcer/tokens_type_command_check.c (4/5)  ✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				check_command(t_tokens *current, t_tokens *head);
bool				check_argument(t_tokens *current, t_tokens *head);
bool				try_set_as_command(t_tokens *current, t_tokens *head);
bool				try_set_as_argument(t_tokens *current, t_tokens *head);

// src/parcer/tokens_type_redir_check.c (/5)  ✅_NORMA + ⚠️_testeo +✅_DESCRIPCIONES + ✅_se_usa
bool				is_redir(t_tokens *current);
t_type				check_redir(t_tokens *current);
bool				invalid_after_redir(t_tokens *current, t_tokens *head, t_type expected_redir);
bool				is_after_redir_with_path(t_tokens *current, t_tokens *head, t_type expected_redir);
void				assign_redirection_type( t_tokens *current, t_tokens *head);

// src/parcer/tokens_type_redir_context_check.c (/5)  ✅_NORMA + ⚠️_testeo +✅_DESCRIPCIONES + ✅_se_usa
bool				is_cmd_aft_redir_wit_prev_cmd(t_tokens *current, t_tokens *prev, t_tokens *head);
bool				is_command_after_redir(t_tokens *current, t_tokens *prev);
bool				is_command_after_pipe(t_tokens *current, t_tokens *prev);
bool				any_command_before(t_tokens *node, t_tokens *head);
bool				is_first_command(t_tokens *current);

// src/parcer/tokens_type_pipe_check.c (/5)  ✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				try_set_as_pipe(t_tokens *current, t_tokens *next_token);
bool				check_pipe(t_tokens *current);
//--check_type_tokens_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


//--check_bash_errors_token──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// src/parcer/syntax_error_token_utils.c (6/5)	✅_NORMA  + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				is_problematic_redir_pattern(t_tokens *tokens);
bool				is_pipe_heredoc_at_start(t_tokens *tokens);
bool				check_bash_before_here(t_tokens *tokens);

// src/parcer/syntax_error_token.c (6/5) ✅_NORMA  + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
bool				check_bash_pipe_before_here(t_tokens *tokens);
bool				check_open_pipe_token(t_tokens *tokens);
bool				check_bash_error_token(t_tokens *tokens);
bool				check_bash_after_here(t_tokens *tokens);
bool				check_bash_pipe_error(t_tokens *tokens, t_program *program);
//--check_bash_errors_token_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


//--VARS──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔# 
// src/parcer/var_lookup.c (/5)	✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
int					extract_var_name(const char *str, char *var_name);
char				*expand_var(t_program *program, char *var_name);

// src/parcer/var_expand_core.c (/5)	✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
char				*process_character(const char *original, t_metachars *meta, char *new_str, int *new_size);
void				handle_expansion(const char *original, t_metachars *meta, char **new_str, int *new_size);
bool				append_var_value_to_str(char **new_str, int *new_size, char *var_value, t_metachars *meta);
char				*get_valid_var_value(t_program *program, const char *original, t_metachars *meta, int *len);
char				*update_string_size(char *new_str, int *new_size, int min_extra_size);

// src/parcer/var_check_expand.c (/5)	✅_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se_usa
char				*init_new_str( t_program *program, int *size);
bool				has_space_for_null_terminator( t_program *program, t_metachars *meta, int size);
char				*build_new_content(const char *original, t_metachars *meta, t_program *program);
bool				check_and_expand_var(t_tokens *tokens, char **str, t_metachars *meta, t_program *program);
bool				check_token_has_var(t_tokens *tokens, t_metachars *meta, t_program *program);
//--VARS_END──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

#endif