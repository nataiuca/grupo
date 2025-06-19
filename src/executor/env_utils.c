
# include "minishell.h"

//--------------------- lista enlazada t_env a **program->envp_copy = **matrix ---------------------//
char	**malloc_env_array(t_env *env, t_program *program) //malloc para lista a matrix
{
	int		count;
	char	**matrix;

	count = env_list_length(env);
	matrix = malloc(sizeof(char *) * (count + 1));
	if (!matrix)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	return (matrix);
}

char	**build_envp_from_list(t_env *env, t_program *program) //vuleve convertir lista enlazada a **matrix (se usa en cd y child -> exec_cmd)
{
	char	**matrix;
	char	*temp;
	int		i;

	matrix = malloc_env_array(env, program);
	if (!matrix)
		return (NULL);
	i = 0;
	while (env)
	{
		temp = ft_strjoin(env->key, "=");
		if (env->value)
			matrix[i] = ft_strjoin(temp, env->value);
		else
			matrix[i] = ft_strdup(temp); // solo "KEY="
		free(temp);
		if (!matrix[i]) // control de error
			return (free_split_strs(matrix), NULL);
		i++;
		env = env->next;
	}
	matrix[i] = NULL;
	return (matrix);
}


//--------------------- aux f() ---------------------//
int	env_list_length(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

int	env_array_size(char **envp)
{
	int	size;

	size = 0;
	if (!envp)
		return (0);
	while (envp[size])
		size++;
	return (size);
}

void update_envp_copy(t_program *program) //actualiza envp_copy
{
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: entrando den update_envp  \033[0m\n");
	if (!program)
		return;
	if (program->envp_copy)
		free_split_strs(program->envp_copy);

	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: envp_copy supuestamente acrtualizado \033[0m\n");
	program->envp_copy = build_envp_from_list(program->env, program);
	if (!program->envp_copy)
	{
		//fprintf(stderr,"update_envp\n");  //testeo
		return;
	}
}