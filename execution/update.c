#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

void close_fds(int fd[][2], int cmd)
{
    int i;
    int k;

    i = -1;
    while (++i < cmd)
    {
        k = -1;
        while (++k < 2)
            close(fd[i][k]);
    }
}

void    excute_childs(int fd[][2], char *const *args, char *paths, int k, int cmd)
{   
        dup2(fd[k][0], STDIN_FILENO);      
        dup2(fd[k + 1][1], STDOUT_FILENO);
        close_fds(fd, cmd);
        execve(paths, args, NULL);
        perror("execv");
        exit(EXIT_FAILURE);
}

int main()
{
    int fd[4][2];
    int cmd = 4;
    int i;
    pid_t pid[4];
    char * const **triple;
    char **paths;

    char *const argv1[] = {"ls", "-l", NULL};
    char *const argv2[] = {"grep", "minishell", NULL};
    char *const argv3[] = {"wc", NULL};
    char *const argv4[] = {"grep", "-n", "9", NULL};

    char *path1 = "/bin/ls";
    char *path2 = "/usr/bin/grep";
    char *path3 = "/bin/wc";
    char *path4 = "/usr/bin/grep";

    triple = malloc(sizeof(char **) * (cmd + 1));
    paths = malloc(sizeof(char *) * (cmd + 1));
    while (1)
    {
        paths[0] = path1;
        paths[1] = path2;
        paths[2] = path3;
        paths[3] = path4;
        paths[4] = NULL;
        break;
    }
    while (1)
    {
        triple[0] = argv1;
        triple[1] = argv2;
        triple[2] = argv3;
        triple[3] = argv4;
        triple[4] = NULL;
        break;
    }
   
    i = -1;
    while (++i < cmd)
        pipe(fd[i]);

    int k = 0;
    int j = 0;
    i = -1;
    while (++i < cmd)
    {
        pid[k] = fork();
        if (pid[k] == -1)
            perror("fork");
        else if (pid[k] == 0){
            excute_childs(fd, triple[k], paths[k], k, cmd);
            exit(EXIT_FAILURE);
        }
        k++;
        j++;
    }

    i = -1;
    while (++i < cmd)
        close(fd[i][1]);
    i = -1;
    while (++i < cmd)
       waitpid(pid[i], NULL, 0);

    char buff[10];
    int bytes_read;
    while ((bytes_read = read(fd[3][0], buff, sizeof(buff))) > 0) {
        write(STDOUT_FILENO, buff, bytes_read);
    }
    close_fds(fd, cmd);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    
}
    // int pid1 = fork();
    // if (pid1 == 0)
    // {
    //     dup2(fd[0][1], STDOUT_FILENO);
    //     close_fds(fd, cmd);
    //     execve(path1, argv1, NULL);
    //     perror("execv");
    //     exit(EXIT_FAILURE);
    // }

    // int pid2 = fork();
    // if (pid2 == 0)
    // {
    //     dup2(fd[0][0], STDIN_FILENO);      
    //     dup2(fd[1][1], STDOUT_FILENO);
    //     close_fds(fd, cmd);
    //     execve(path2, argv2, NULL);
    //     perror("execv");
    //     exit(EXIT_FAILURE);
    // }

    // int pid3 = fork();
    // if (pid3 == 0)
    // {
    //     dup2(fd[1][0], STDIN_FILENO);
    //     dup2(fd[2][1], STDOUT_FILENO);
    //     close_fds(fd, cmd);
    //     execve(path3, argv3, NULL);
    //     perror("execv");
    //     exit(EXIT_FAILURE);
    // }

    // int pid4 = fork();
    // if (pid4 == 0)
    // {
    //     dup2(fd[2][0], STDIN_FILENO);
    //     dup2(fd[3][1], STDOUT_FILENO);
    //     close_fds(fd, cmd);
    //     execve(path4, argv4, NULL);
    //     perror("execv");
    //     exit(EXIT_FAILURE);
    // }