
# include "minishell.h"

static void set_or_add_env_var(t_program *program, t_env **env, const char *key, const char *value) //✅
{
	t_env *current;
	t_env *new_node;

	if (!key || !value || !env)
		return;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));				// no se encontró la variable, hay que crearla
	if (!new_node)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return;
	}
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
}

static int change_dir_and_get_cwd(char **curr, char *pwd, t_program *program) //✅
{
	struct stat path_stat;

	if (!pwd)
		return (ft_error(program, MSG_ERR_MISS_ARG, "cd", 1), 1); //REVISAR ESTE ERROR

	if (stat(pwd, &path_stat) == 0 && S_ISREG(path_stat.st_mode))
		return (ft_error(program, MSG_ERR_NOT_IS_DIRECTORY, pwd, 1), 1);
	if (chdir(pwd) != 0)
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(pwd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		return (program->last_exit_status = 1, 1);
	}
	*curr = getcwd(NULL, 0);
	if (!*curr)
	{
		ft_putstr_fd("bash: cd: getcwd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		return (program->last_exit_status = 1, 1);
	}
	return (0);
}

static int update_dir(char *pwd, t_program *program)//✅
{
	char *curr;
	char *old_pwd;

	old_pwd = ft_get_env_aux(program->env, "PWD"); // puede ser NULL
	if (change_dir_and_get_cwd(&curr, pwd, program))
		return (1);

	if (old_pwd)
		set_or_add_env_var(program, &program->env, "OLDPWD", old_pwd);

	if (curr)
	{
		set_or_add_env_var(program, &program->env, "PWD", curr);
		free(curr);
	}
	return (0);
}

static char *prepare_pwd(t_all *all, t_program *program) //✅
{
	char *pwd;

	pwd = NULL;
	if (!all->exec->args[1])			// ❌ Sin argumentos: no permitido
	{
		ft_error(program, MSG_ERR_MISS_ARG, "cd", 1);
		return NULL;
	}

	if (ft_strcmp(all->exec->args[1], "-") == 0) 	// ❌ "cd -" no soportado
	{
		ft_error(program, MSG_ERR_INV_OPT, "cd: -", 1);
		return NULL;
	}
		
	if (all->exec->args[2]) 	// ❌ Demasiados argumentos
	{
		ft_error(program, MSG_ERR_TOO_MANY_ARGS, "cd", 1);
		//printf("return en cd\n");
		return NULL;
	}
		
	pwd = ft_strdup(all->exec->args[1]);  	// ✅ Argumento válido
	if (!pwd)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return NULL;
	}
	//program->last_exit_status=0;
	return (pwd);
}

int ft_cd(t_all *all, t_program *program) //✅ 
{
	char *pwd;

	pwd = prepare_pwd(all, program);
	if (!pwd) // Si es NULL, el error ya se reportó en prepare_pwd
		return 1;

	if (update_dir(pwd, program))
	{
		free(pwd);
		return 1;
	}
	update_envp_copy(program);
	free(pwd);
	//program->last_exit_status = 0; NOT
	return (0);
}
