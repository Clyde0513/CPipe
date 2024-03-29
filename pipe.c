#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// File descriptor: 0 (stdin), 1 (stdout), 2 (stderr)
// fd[0]: read end of the pipe
// fds[1]: write end of the pipe

int main(int argc, char *argv[])
{
  int pipe_fds[7][2]; // can hold up to 8 arguments or more
  int n = argc - 1;   // up to argc - 1 processes
  pid_t ret = 0;

  if (argc == 1) // if it's only ./pipe, do nothing
  {
    return EINVAL;
  }
  // calling pipe first on all the new processes up to n - 1.
  for (int i = 0; i < n - 1; i++)
  {
    if (pipe(pipe_fds[i]) == -1)
    {
      perror("pipe failed");
      exit(EXIT_FAILURE);
    }
  }
  // then call fork() second on all the children up to n children processes
  for (int i = 0; i < n; i++)
  {
    if ((ret = fork()) == -1)
    {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
    if (ret == 0) // children
    {
      if (i != 0) // not in the first process
      {
        if (dup2(pipe_fds[i - 1][0], 0) == -1) // redirect stdin to read in
        {
          perror("dup2 failed");
          exit(EXIT_FAILURE);
        }
      }
      if (i != n - 1) // not in the last process
      {
        if (dup2(pipe_fds[i][1], 1) == -1) // redirect stdout to write in
        {
          perror("dup2 failed");
          exit(EXIT_FAILURE);
        }
      }
      for (int i = 0; i < n - 1; i++)
      {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
      }
      if (execlp(argv[i + 1], argv[i + 1], NULL) == -1)
      {
        perror(argv[i + 1]);
        exit(EXIT_FAILURE);
      }
    }
  }

  // parent process
  int success = EXIT_SUCCESS;
  int status = 0;
  int true_status = 0;

  for (int i = 0; i < n - 1; i++)
  {
    close(pipe_fds[i][0]);
    close(pipe_fds[i][1]);
  }
  // need to wait for other process to be done.
  // waiting before exiting the parent
  for (int i = 0; i < n; i++)
  {
    while ((waitpid(ret, &status, 0)) > 0)
    {
      // waitpid(ret, &status, 0);
      if (status != 0)
      {
        true_status = status;
      }
      if ((WEXITSTATUS(true_status) || !WIFEXITED(true_status)) != EXIT_SUCCESS)
      {
        //     return -1;
        success = EXIT_FAILURE;
      }
    }
  }
  exit(success);
}
