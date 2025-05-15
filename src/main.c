/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omkuzu <omkuzu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:28:29 by omkuzu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 100

int main(void) 
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    while (1) 
    {
        printf("minishell> ");
        nread = getline(&line, &len, stdin);
        if (nread == -1) 
        {
            printf("\n");
            break;
        }
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';
        if (line[0] == '\0')
            continue;
        char *args[MAX_ARGS];
        int argc = 0;
        char *token = strtok(line, " ");
        while (token && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;
        if (strcmp(args[0], "exit") == 0)
            break;
        if (strcmp(args[0], "cd") == 0) {
            char *target = args[1] ? args[1] : getenv("HOME");
            if (chdir(target) != 0)
                perror("minishell: cd");
            continue;
        }
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("minishell: fork");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("minishell");
            exit(EXIT_FAILURE);
        } else 
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    free(line);
    return 0;
}
