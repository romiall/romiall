#include "fg.h"

#include "headers.h"

int tichnas_fg(int pid) {
  int status;
  setpgid(pid, getpgid(0));

  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);

  tcsetpgrp(0, pid);

  if (kill(pid, SIGCONT) < 0) perror("Error");

  waitpid(pid, &status, WUNTRACED);

  tcsetpgrp(0, getpgid(0));

  signal(SIGTTIN, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);

  if (WIFSTOPPED(status)) return 0;

  return WIFEXITED(status) ? pid : -pid;
}