#include "kjob.h"

#include "headers.h"

int tichnas_kjob(int pid, int sig) {
  if (kill(pid, sig) < 0) {
    perror("Error");
    return -1;
  }

  return 0;
}