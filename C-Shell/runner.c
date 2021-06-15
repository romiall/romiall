#include "runner.h"

#include "headers.h"

int runCommand(int background, char** args) {
  int pid = fork();

  if (pid < 0) {
    perror("Error");
    return -1;

  } else if (pid) {
    if (background) {
      fprintf(stderr, "%s started with PID %d\n", args[0], pid);

      return pid;

    } else {
      int status;

      signal(SIGTTIN, SIG_IGN);
      signal(SIGTTOU, SIG_IGN);

      tcsetpgrp(0, pid);

      waitpid(pid, &status, WUNTRACED);

      tcsetpgrp(0, getpgid(0));

      signal(SIGTTIN, SIG_DFL);
      signal(SIGTTOU, SIG_DFL);

      if (WIFSTOPPED(status)) return pid;

      if (WEXITSTATUS(status) == EXIT_FAILURE) return -1;
    }

  } else {
    setpgid(0, 0);

    if (execvp(args[0], args) < 0) {
      perror("Error");
      exit(1);
    }
  }

  return 0;
}
