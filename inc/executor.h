/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:49:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/19 10:34:05 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes minishell:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "datatypes.h"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Prototypes:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
//--here_doc──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// src/executor/here_doc.c (/5): // ✅_NORMA  + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se usa
int					open_here_doc_file(t_program *program, t_here *here, int i);
//static bool		is_delimiter(char *line, char *delimiter);
int					process_here_doc_lines(t_all *all, t_program *program, t_tokens *temp, int i);
void				here_doc(t_all *all, t_program *program, t_here *here);
void				check_here_doc(t_all *all, t_program *program, t_here *here);

// src/executor/here_doc_utils.c (/5): // ✅_NORMA  + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se usa
void				count_here_doc(t_tokens *tokens, t_here *here);
void				generate_here_doc_name(t_program *program, t_here *here, int i, char *itoa_str);
void				write_and_free_here_line(char *line, int fd);
bool				alloc_here_doc_names(t_here *here, t_program *program);
void				free_here_doc_names(t_here *here, int count);
//--here_doc end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


//------------------ EJECUCIÓN -------------------//
// src/executor/built_args.c (5/5)    ⚠️_NORMA  + ⚠️_DESCRIPCIONES + ⚠️_testeo  + ⚠️_testeo_free + ✅_se usa
int					args_split_len(t_tokens *curr);
bool				extract_args(t_tokens *curr, char **args);
char				**split_args(t_tokens *tokens, t_program *program);
bool				update_args(t_exec *exec, t_tokens *tokens, t_program *program);

// src/executor/env_utils.c (5/5)   ⚠️_NORMA  + ⚠️_DESCRIPCIONES + ⚠️_testeo 
char				**malloc_env_array(t_env *env, t_program *program);
char				**build_envp_from_list(t_env *env, t_program *program);
int					env_list_length(t_env *env);
int					env_array_size(char **envp);
void				update_envp_copy(t_program *program); //export, unset, execve, cd

// src/executor/env_init.c (/5)   ⚠️_NORMA  + ⚠️_DESCRIPCIONES + ⚠️_testeo 
char				**clone_env_array(char **envp, t_program *program);
t_env				*init_env_node(t_program *program);
t_env				*parse_env_var_to_node(char *env_var, t_program *program);
t_env				*envp_array_to_list(char **envp, t_program *program);


// src/executor/pipes.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
void				close_all_pipe_fds_in_parent(t_exec *exec);
int					init_pipe(t_program *program, t_exec *exec, int i);
int					alloc_pipe_fd_array(t_program *program, t_exec *exec);
int					alloc_pipe_pair_at_index(t_program *program, t_exec *exec, int i);
bool				init_all_pipes(t_program *program, t_all *all);
//--------------------- FREE PIPES ---------------------//
void				free_all_pipes(t_exec *exec);
void				free_pipes_up_to_index(t_exec *exec, int limit);
void				close_all_pipes(t_exec *exec);
void				close_unused_pipes_in_parent(t_exec *exec, int i);

// src/executor/redirections.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
bool				config_fd_redir(int infile, int outfile, t_program *program);
int					handle_output(t_program *program, t_exec *exec, char *file, int flags);
int					handle_input(t_program *program, t_exec *exec, char *file);
int					get_output_flags(int type);
int					process_output_redirection(t_program *program, t_exec *exec, t_tokens *curr);
int					process_input_redirection(t_all *all, t_program *program, t_tokens *curr, int *heredoc_index);
bool				apply_redir(t_all *all, t_program *program);
//static bool		redirect_input(t_exec *exec, int i, t_program *program);
//static bool		redirect_output(t_exec *exec, int i, t_program *program);
bool				apply_cmd_redir(t_exec *exec, int i, t_program *program);


// src/executor/get_path_execve.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//---------------- HANDLE_ERRORS------------------//
int					handle_access_errors(t_program *program, t_all *all, char *cmd);
int					validate_executable(t_program *program, t_all *all, char *cmd_path);
//------------------ GET PATH --------------------//
char				*get_cmd_full_path(char *cmd);
char				*check_path(char *path, char *cmd);
char				**extract_path_dirs(char **envp);
char				*find_cmd_in_paths(t_program *program, char *cmd);
char				*resolve_command_path(t_tokens *curr, t_all *all, t_program *program);
//------------------- EXECVE ---------------------//
void				free_and_exit_child(t_all *all, t_program *program, int exit_status);
void				execute_command(t_all *all, t_program *program, t_tokens *curr);


// src/executor/executor_loop.c (5/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//static int		init_executor(t_program *program, t_all *all);
//static int		handle_fork_error(t_program *program, t_all *all, t_tokens *original, int i);
//static int		should_skip_command(t_tokens **curr, t_tokens **cmd_token);
//static int		prepare_and_fork_command(t_program *program, t_all *all, t_tokens *cmd_token, int i);
//static void		advance_to_next_command(t_tokens **curr);
int					fork_command_loop(t_program *program, t_all *all);
//static void		finalize_executor(t_all *all, t_program *program);
int					executor_loop(t_all *all, t_program *program);

// src/executor/executor.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES + //❌ revisar check_failed_redirection_child con las señales
bool				must_use_env_builtin(char *cmd);
bool				is_forbidden_env_path(char *cmd);
bool				must_use_pwd_builtin(char *cmd);
bool				is_forbidden_pwd_path(char *cmd);
//static void		handle_fork_error(t_program *program, t_all *all);
//static void		exec_and_exit_child(t_all *all, t_tokens *curr, t_program *program, int i);
pid_t				fork_and_exec_command(t_all *all, t_tokens *curr, int i, t_program *program);
int					check_failed_redir_child(pid_t pid, t_program *program); 		//❌ revisar
void				wait_child(t_all *all, t_program *program);
int					check_child(t_program *program, t_all *all);
void				fork_and_run_single_cmd(t_all *all, t_program *program, t_tokens *curr);
void				run_single_command(t_all *all, t_program *program);
void				ft_exec(t_all *all, t_program *program);

// src/executor/executor_utils.c (4/5) ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES + 🚩_revisar si se usa
t_tokens			*find_cmd (t_tokens *tokens);
t_tokens			*skip_redir(t_tokens *curr);
bool				is_empty_string(char *s);
bool				is_empty_cmd(char *s);
int					count_type_tokens(t_tokens *tokens, t_type type_to_find);
bool				init_exec_vals(t_program *program, t_all *all);




//👽👽-----V2:BUILTINGS-----👽👽//

//builtins/builtings.c		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
bool				check_builtin(char *cmd);
int					ft_builtin(t_all *all, t_program *program);

// src/builtins/exit.c (3/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//static int		is_valid_numeric_arg(char *arg);
//static int		check_exit_args(t_exec *exec, t_program *program);
int					ft_exit(t_all *all, t_program *program);

// src/builtins/echo.c (5/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//static int		check_flag(char *str);
//static bool		check_arg_max(t_program *program, char **args, int start_index);
//static int		skip_echo_flags(char **args, int *print_newline);
//static void		print_echo_args(char **args, int i, int out);
int					ft_echo(t_program *program, t_exec *exec);

// src/builtins/cd.c (5/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//static void		update_env_var(t_program *program, t_env **env, const char *key, const char *value); //solo cd
//static int		get_dirs(char **curr, char *pwd, t_program *program); //solo cd
//static int		change_dir(char *pwd, t_program *program); //solo cd
//static char		*prepare_pwd(t_all *all, t_program *program); //solo cd
int					ft_cd(t_all *all, t_program *program); //solo cd

// src/builtins/pwd.c (2/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
char				*ft_get_env_aux(t_env *env, const char *key);	//cd y pwd
//static int		check_pwd_error(t_program *program, char **args);
int					ft_pwd(t_all *all, t_program *program, char **args);

// src/builtins/env.c (4/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//static void		print_env(t_env *env, int out_fd);
//static bool		has_valid_path(t_env *env);
//static int		check_env_error(t_program *program, char **args);
int					ft_env(t_program *program, t_all *all, char **args);


// src/builtins/export_utils_2.c (8/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
int					init_env_key(t_env *node, char *key);
int					init_env_value_flags(t_env *node, char *value);
t_env				*create_env_node_from_key_value(char *key, char *value);
void				append_env_node(t_env **env, t_env *new_node);
void				append_new_env_var(t_env **env, char *key, char *value);		//solo export

// src/builtins/export_utils.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
t_env				*search_env_key(t_env *env, char *key);					//export & vars_parcer
void				set_env_value_by_key(t_env *env, char *key, char *new_value);		//solo export
void				parse_env_assignment(char *str, char **key, char **value);	//solo export
void				print_export_identifier_error(char *arg, t_program *program);
void				print_export_error(t_program *program, char **args);

// src/builtins/export.c (7/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
void				write_export(t_env *env, int fd);
int					contains_equal(char *arg);
void				update_or_add_env(t_program *program, char *key, char *value, int has_equal);
void				parse_and_export_var(t_program *program, char *arg);
int					ft_export(t_all *all, char **args, t_program *program);


// src/builtins/unset.c (/5)		⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
int					check_key(char *str);										//unset y export
int					delete_env_node(t_env **env, const char *key);
//static void		print_unset_error(char *arg, t_program *program);
int					ft_unset(t_all *all, char **args, t_program *program);


#endif