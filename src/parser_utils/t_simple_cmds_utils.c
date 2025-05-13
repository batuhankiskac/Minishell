#include "../../include/utils.h"

t_simple_cmds	*ft_simple_cmdsnew(char **str,
	int num_redirections, t_lexer *redirections)
{
	t_simple_cmds	*new_element;

	new_element = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	if (!new_element)
		return (0);
	new_element->str = str;
	new_element->builtin = builtin_arr(str[0]);
	new_element->hd_file_name = NULL;
	new_element->num_redirections = num_redirections;
	new_element->redirections = redirections;
	new_element->next = NULL;
	new_element->prev = NULL;
	return (new_element);
}

void	ft_simple_cmdsadd_back(t_simple_cmds **lst, t_simple_cmds *new)
{
	t_simple_cmds	*tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

void	ft_simple_cmds_rm_first(t_simple_cmds **lst)
{
	t_simple_cmds	*tmp;

	if (!*lst)
		return ;
	tmp = (*lst)->next;
	ft_lexerclear(&(*lst)->redirections);
	free(*lst);
	*lst = tmp;
}

void	ft_simple_cmdsclear(t_simple_cmds **lst)
{
	t_simple_cmds	*tmp;
	t_lexer			*redirections_tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		redirections_tmp = (*lst)->redirections;
		ft_lexerclear(&redirections_tmp);
		if ((*lst)->str)
			free_arr((*lst)->str);
		if ((*lst)->hd_file_name)
			free((*lst)->hd_file_name);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

t_simple_cmds	*ft_simple_cmdsfirst(t_simple_cmds *map)
{
	int	i;

	i = 0;
	if (!map)
		return (NULL);
	while (map->prev != NULL)
	{
		map = map->prev;
		i++;
	}
	return (map);
}


int dummy_builtin(t_tools *tools, t_simple_cmds *simple_cmd)
{
    (void)tools;
    (void)simple_cmd;
    return (0);
}

int (*builtin_arr(char *str))(t_tools *tools, t_simple_cmds *simple_cmd)
{
    (void)str;
    return (&dummy_builtin);
}


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/minishell.h"

void single_cmd(t_simple_cmds *cmds, t_tools *tools)
{
    pid_t pid;

    if (!cmds || !cmds->str || !cmds->str[0])
        return;

    pid = fork();
    if (pid == 0)
    {
        execve(cmds->str[0], cmds->str, tools->envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        perror("fork");
    }
}
