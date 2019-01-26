/*
** EPITECH PROJECT, 2019
** PSU_minishell1_2018
** File description:
** minishell_command
*/

#include "my.h"

void display_command_errors(char *program_name, int status)
{
    if (my_strncmp(program_name, "./", 2) == 0)
        wait(&status);
    if (status == 9 || status == 134)
        my_putstr("Segmentation fault\n");
    else if (status != 0) {
        my_putstr(program_name);
        my_putstr(": Command not found.\n");
    }
}

void do_command(char **path, char *program_name, char *const *args)
{
    int c_pid = fork();
    int status;
    char *tmp;

    if (c_pid == 0 && path[0] != NULL &&
    my_strncmp(program_name, "./", 2) != 0) {
        for (int i = 0; path[i] != NULL; i++) {
            tmp = malloc(sizeof(char) * 20);
            tmp = my_strcat(path[i], "/");
            execve(my_strcat(tmp, program_name), args, NULL);
            free(tmp);
        }
    } else if (c_pid > 0) {
        if (path[0] != NULL && my_strncmp(program_name, "./", 2) != 0)
            wait(&status);
        kill(c_pid, SIGKILL);
    } else {
        if (path[0] != NULL && my_strncmp(program_name, "./", 2) != 0)
            perror("fork failed");
    }
    display_command_errors(program_name, status);
}

void find_command_3(struct data data)
{
    if (my_strcmp(data.program_name, "unsetenv") == 0) {
        if (data.args[1] == NULL) {
            my_putstr("unsetenv: Too few arguments.\n");
            return;
        }
        if (my_strncmp(data.args[1], "PATH", 4) == 0) {
            data.path[0] = NULL;
            data.env = rm_path(data);
        }
        return;
    } else
        do_command(data.path, data.program_name, data.args);
}

void find_command_2(struct data data)
{
    if (my_strcmp(data.program_name, "setenv") == 0) {
        if (data.nbr_args == 0) {
            print_env(data.env);
            return;
        }
        if (my_strncmp(data.args[1], "PATH", 4) == 0) {
            data.path = modify_path(data);
            return;
        }
    }
    find_command_3(data);
}

void find_command(struct data data)
{
    char pwd[128];

    getcwd(pwd, sizeof(pwd));
    if (my_strcmp(data.program_name, "exit") == 0) {
        my_putstr("exit\n");
        exit(0);
    }
    if (my_strcmp(data.program_name, "pwd") == 0) {
        my_putstr(pwd);
        my_putchar('\n');
        return;
    }
    if (my_strcmp(data.program_name, "cd") == 0) {
        chdir(data.args[1]);
        return;
    }
    if (my_strcmp(data.program_name, "env") == 0) {
        print_env(data.env);
        return;
    }
    find_command_2(data);
}