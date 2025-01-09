/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/08 15:07:16 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i])
			free(table[i++]);
		free(table);
		table = NULL;
	}
}

int	ft_len_tab(char **env, char *var)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (env[i])
		len += ft_strlen(env[i++]);
	len += ft_strlen(var);
	return (len);
}

int	ft_argc(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

char	**ft_preparss(char **argv);

char	*ft_get_var(char *str)
{
	char	*var;
	int		i;

	i = 0;
	while (str[i] != '=')
		i++;
	var = ft_substr(str, 0, i);
	return (var);
}

char	*my_getenv(const char *name, char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j])
			j++;
		tmp = ft_substr(env[i], 0, j);
		if (ft_strncmp(tmp, name, j) == 0 && name[j] == '\0')
		{
			free(tmp);
			return (env[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	**ft_new_env(char **argv, char **env)
{
	int		i;
	int		j;
	char	**new_env;
	char	*var;

	i = 0;
	new_env = malloc(sizeof(char *) * (ft_argc(env) + ft_argc(argv)));
	if (!new_env)
	{
		printf("Error malloc\n");
		ft_free_tab(argv);
		return (env);
	}
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		// printf("%s\n", new_env[i]);
		if (!new_env[i])
		{
			ft_free_tab(new_env);
			ft_free_tab(argv);
			return (env);
		}
		i++;
	}
	j = 1;
	while (argv[j])
	{
		var = ft_get_var(argv[j]);
		char *path = my_getenv(var, env);
		printf("path: %s | var: %s\n", path, var);
		if (!path)
			new_env[i] = ft_strdup(argv[j]);
		else
			printf("%s found\n", var);
		printf("%s\n", new_env[i]);
		if (!new_env[i])
		{
			ft_free_tab(new_env);
			ft_free_tab(argv);
			return (env);
		}
		if (!getenv(var))
			i++;
		j++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	excution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	path = ft_strjoin("/usr/bin/", argv[0]);
	if (!path)
	{
		ft_free_tab(argv);
		return ;
	}
	if (!access(path, X_OK))
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			execve(path, argv, env);
		while (wait(NULL) > 0)
			;
	}
	free(path);
}

char	**ft_export(char **argv, char **env)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 1;
	if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])) && ft_argc(argv) > 1
		&& env)
	{
		argv = ft_preparss(argv);
		if (!argv)
		{
			printf("Error argv\n");
			return (env);
		}
		new_env = ft_new_env(argv, env);
		// ft_free_tab(argv);
		return (new_env);
	}
	else
		excution_cmd(argv, env);
	ft_free_tab(argv);
	return (NULL);
}

char	**ft_preparss(char **argv)
{
	int		i;
	int		j;
	int		k;
	int		nb_valid;
	char	**new_argv;

	nb_valid = 0;
	i = 1;
	j = 0;
	k = 0;
	if (!argv)
		return (NULL);
	while (argv[j])
	{
		i = 1;
		while (argv[j][i] != '=' && argv[j][i])
			i++;
		if (argv[j][i] != '=' && !getenv(ft_get_var(argv[j])))
			nb_valid++;
		j++;
	}
	j = 0;
	i = 0;
	// printf("%d\n", nb_valid + 2);
	new_argv = malloc(sizeof(char *) * (nb_valid + 2));
	if (!new_argv)
	{
		ft_free_tab(argv);
		return (NULL);
	}
	new_argv[k++] = ft_strdup(argv[j++]);
	while (argv[j])
	{
		i = 1;
		while (argv[j][i] != '=' && argv[j][i])
			i++;
		if (argv[j][i] == '=' && !getenv(ft_get_var(argv[j])))
			new_argv[k++] = ft_strdup(argv[j]);
		j++;
	}
	new_argv[k] = NULL;
	ft_free_tab(argv);
	// int w = 0;
	// while (new_argv[w])
	// 	printf("%s\n", new_argv[w++]);
	return (new_argv);
}

char	**ft_strdup_tab(char **str)
{
	int		i;
	char	**result;

	i = 0;
	result = malloc(sizeof(char *) * (ft_argc(str) + 1));
	if (!result)
		return (NULL);
	while (str[i])
	{
		result[i] = ft_strdup(str[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	main(int argc, char **argv, char **env)
{
	char *input;
	char **new_env;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	new_env = ft_strdup_tab(env);
	while (input)
	{
		input = readline("Minishell> ");
		if (input && input[0])
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				env = new_env;
				// ft_free_tab(new_env);
				new_env = NULL;
				new_env = ft_export(argv, env);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
	ft_free_tab(new_env);
}