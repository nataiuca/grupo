/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:49:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/12 12:21:36 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

//#➵⤐──╌╌➣⋆➣── Includes minishell ──➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#

# include "datatypes.h"

//#➵⤐──╌╌➣⋆➣── Here_doc ──────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/executor/here_doc.c
int			open_here_doc_file(t_program *program, t_here *here, int i);
void		close_fd(t_here *here, int i);
int			process_here_lines(t_all *all, t_program *program, t_tokens *temp,
				int i);
;
bool		here_doc(t_all *all, t_program *program, t_here *here,
				t_tokens *tokens);
bool		check_here_doc(t_all *all, t_program *program, t_here *here);

// src/executor/here_doc_utils.c
void		count_here_doc(t_tokens *tokens, t_here *here);
void		generate_here_doc_name(t_program *program, t_here *here, int i,
				char *itoa_str);
void		write_and_free_here_line(char *line, int fd);
bool		alloc_here_doc_names(t_here *here, t_program *program);
void		free_here_doc_names(t_here *here, int count);

// src/executor/here_process.c
void		init_here_loop_state(char **line, bool *reached, int *lineno,
				t_program *prog);
int			check_signal_and_line(t_program *program, char *line);
int			process_delimiter(char *line, char *delim, bool *reached);
int			process_and_write_line(t_all *all, t_program *program,
				t_tokens *temp, int i);
int			finalize_here_loop(char *delim, bool reached, int lineno);

//#➵⤐──╌╌➣⋆➣── Execution ─────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/executor/built_args.c
int			args_split_len(t_tokens *curr);
bool		extract_args(t_tokens *curr, char **args);
char		**split_args(t_tokens *tokens, t_program *program);
bool		update_args(t_exec *exec, t_tokens *tokens, t_program *program);

// src/executor/env_utils.c
char		**malloc_env_array(t_env *env, t_program *program);
char		**build_envp_from_list(t_env *env, t_program *program);
int			env_list_length(t_env *env);
int			env_array_size(char **envp);
void		update_envp_copy(t_program *program);

// src/executor/env_init.c
char		**clone_env_array(char **envp, t_program *program);
t_env		*init_env_node(t_program *program);
t_env		*parse_env_var_to_node(char *env_var, t_program *program);
t_env		*envp_array_to_list(char **envp, t_program *program);

// src/executor/pipes.c
int			init_pipe(t_program *program, t_exec *exec, int i);
int			alloc_pipe_fd_array(t_program *program, t_exec *exec);
int			alloc_pipe_pair_at_index(t_program *program, t_exec *exec, int i);
bool		init_all_pipes(t_program *program, t_all *all);

// src/executor/pipes_free.c
void		close_all_pipe_fds_in_parent(t_exec *exec);
void		free_all_pipes(t_exec *exec);
void		free_pipes_up_to_index(t_exec *exec, int limit);
void		close_unused_pipes_in_parent(t_exec *exec, int i);
void		close_unused_pipes(t_exec *exec, int curr_cmd_idx);

// src/executor/redirections.c
int			get_output_flags(int type);
bool		config_fd_redir(int infile, int outfile, t_program *program);
// static bool	redirect_input(t_exec *exec, int i, t_program *program);
bool		redirect_output(t_exec *exec, int i, t_program *program);
bool		apply_cmd_redir(t_exec *exec, int i, t_program *program);

// src/executor/redirections_utils.c
int			handle_output(t_program *program, t_exec *exec, char *file,
				int flags);
int			handle_input(t_program *program, t_exec *exec, char *file);
void		close_and_exit(t_all *all, int i, t_program *program,
				int exit_code);
void		handle_check_child_result(t_all *all, t_program *program, int i);

// src/executor/redirections_2.c
int			process_output_redirection(t_program *program, t_exec *exec,
				t_tokens *curr);
int			open_heredoc_file(t_all *all, t_program *program, t_tokens *curr,
				int *heredoc_index);
int			open_input_file(t_all *all, t_program *program, t_tokens *curr);
int			process_input_redirection(t_all *all, t_program *program,
				t_tokens *curr, int *heredoc_index);
bool		apply_redir(t_all *all, t_program *program);

// src/executor/get_path_execve.c
char		*get_cmd_full_path(char *cmd);
char		*check_path(char *path, char *cmd);
char		**extract_path_dirs(char **envp);
char		*find_cmd_in_paths(t_program *program, char *cmd);
char		*resolve_command_path(t_tokens *curr, t_all *all,
				t_program *program);

// src/executor/get_path_exec_utils.c
void		free_and_exit_child(t_all *all, t_program *program,
				int exit_status);
int			handle_access_errors(t_program *program, t_all *all, char *cmd);
int			validate_executable(t_program *program, t_all *all, char *cmd_path);
int			is_direct_path(char *cmd);

// src/executor/executor_fork_cmd_utils.c
int			init_executor(t_program *program, t_all *all);
int			handle_fork_error(t_program *program, t_all *all,
				t_tokens *original, int i);
void		advance_to_next_command(t_tokens **curr);
void		finalize_executor(t_all *all, t_program *program);
int			check_failed_redir_child(pid_t pid, t_program *program);

// src/executor/executor_fork_utils.c
// static void	wait_all_children(t_all *all, int *last_status);
void		handle_signal_output(t_program *program);
void		wait_child(t_all *all, t_program *program);

// src/executor/executor_fork_cmd.c
pid_t		fork_and_exec_command(t_all *all, t_tokens *curr, int i,
				t_program *program);
int			prepare_and_fork_command(t_program *program, t_all *all,
				t_tokens *cmd_token, int i);
int			fork_command_loop(t_program *program, t_all *all);
int			executor_loop(t_all *all, t_program *program);

// src/executor/executor_single_cmd.c
int			check_child(t_program *program, t_all *all);
// static void	handle_parent_process(pid_t pid, t_program *program,
//		int status);
void		fork_and_run_single_cmd(t_all *all, t_program *program,
				t_tokens *curr);
void		execute_command(t_all *all, t_program *program, t_tokens *curr);
void		run_single_command(t_all *all, t_program *program);

// src/executor/executor.c
void		handle_fork_error_2(t_program *program, t_all *all);
void		exec_and_exit_child(t_all *all, t_tokens *curr, t_program *program,
				int i);
int			check_child_fork(t_program *program, int i, t_all *all);
void		ft_exec(t_all *all, t_program *program);

// src/executor/executor_utils.c
t_tokens	*find_cmd(t_tokens *tokens);
t_tokens	*skip_redir(t_tokens *curr);
bool		is_empty_string(char *s);
bool		is_empty_cmd(char *s);
int			count_type_tokens(t_tokens *tokens, t_type type_to_find);

// src/executor/executor_utils_2.c
bool		init_exec_vals(t_program *program, t_all *all);
bool		must_use_env_builtin(char *cmd);
bool		is_forbidden_env_path(char *cmd);
bool		must_use_pwd_builtin(char *cmd);
bool		is_forbidden_pwd_path(char *cmd);

//#➵⤐──╌╌➣⋆➣── Builtins ──────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/builtins/builtins.c
bool		check_builtin(char *cmd);
int			ft_builtin(t_all *all, t_program *program);

// src/builtins/exit.c
// static int		is_valid_numeric_arg(char *arg);
// static int		check_exit_args(t_exec *exec, t_program *program);
int			ft_exit(t_all *all, t_program *program);

// src/builtins/echo.c
// static int		check_flag(char *str);
// static bool		check_arg_max(t_program *program, char **args,
//			int start_index);
// static int		skip_echo_flags(char **args, int *print_newline);
// static void		print_echo_args(char **args, int i, int out);
int			ft_echo(t_program *program, t_exec *exec);

// src/builtins/cd.c
// static void		update_env_var(t_program *program, t_env **env,
//			const char *key, const char *value);
// static int		get_dirs(char **curr, char *pwd, t_program *program);
// static int		change_dir(char *pwd, t_program *program);
// static char		*prepare_pwd(t_all *all, t_program *program);
int			ft_cd(t_all *all, t_program *program);

// src/builtins/pwd.c
char		*ft_get_env_aux(t_env *env, const char *key);
// static int		check_pwd_error(t_program *program, char **args);
// static int		get_pwd_or_fallback(t_program *program, char **args,
//			char **pwd_out);
int			ft_pwd(t_all *all, t_program *program, char **args);

// src/builtins/env.c
int			update_env_var_if_exists(t_env *env, const char *key,
				const char *value);
// static void		print_env(t_env *env, int out_fd);
// static bool		has_valid_path(t_env *env);
// static int		check_env_error(t_program *program, char **args);
int			ft_env(t_program *program, t_all *all, char **args);

// src/builtins/export_utils_2.c
int			init_env_key(t_env *node, char *key);
int			init_env_value_flags(t_env *node, char *value);
t_env		*create_env_node_from_key_value(char *key, char *value);
void		append_env_node(t_env **env, t_env *new_node);
void		append_new_env_var(t_env **env, char *key, char *value);

// src/builtins/export_utils.c
t_env		*search_env_key(t_env *env, char *key);
void		set_env_value_by_key(t_env *env, char *key, char *new_value);
void		parse_env_assignment(char *str, char **key, char **value);
void		print_export_identifier_error(char *arg, t_program *program);
void		print_export_error(t_program *program, char **args);

// src/builtins/export.c
void		write_export(t_env *env, int fd);
int			contains_equal(char *arg);
void		update_or_add_env(t_program *program, char *key, char *value,
				int has_equal);
void		parse_and_export_var(t_program *program, char *arg);
int			ft_export(t_all *all, char **args, t_program *program);

// src/builtins/unset.c
int			check_key(char *str);
int			delete_env_node(t_env **env, const char *key);
// static void		print_unset_error(char *arg, t_program *program);
int			ft_unset(t_all *all, char **args, t_program *program);

#endif