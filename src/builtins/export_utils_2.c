
#include "minishell.h"

int	init_env_key(t_env *node, char *key)  //✅ 
{
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (0);
	}
	return (1);
}

int	init_env_value_flags(t_env *node, char *value)  //✅ 
{
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
		{
			free(node->key);
			free(node);
			return (0);
		}
		node->is_export_only = 0;
	}
	else
	{
		node->value = NULL;
		node->is_export_only = 1;
	}
	return (1);
}

t_env	*create_env_node_from_key_value(char *key, char *value)  //✅ 
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	if (!init_env_key(new_node, key))
		return (NULL);
	if (!init_env_value_flags(new_node, value))
		return (NULL);
	return (new_node);
}

void	append_env_node(t_env **env, t_env *new_node)  //✅ 
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	append_new_env_var(t_env **env, char *key, char *value) //✅ 
{
	t_env	*new_node;

	new_node = create_env_node_from_key_value(key, value);
	if (!new_node)
		return ;
	append_env_node(env, new_node);
}
