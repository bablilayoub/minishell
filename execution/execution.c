
void execution(t_cmd *cmd)
{
    int fd[2];
    pid_t pid;
    
  
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execve(cmd->cmd, cmd->args, NULL);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execve(cmd->cmd, cmd->args, NULL);
    }
}
int main()
{
    t_cmd cmd;
    execution(&cmd);
    return 0;