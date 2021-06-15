#include "bg.h"

#include "headers.h"
//fait tourner un processus en arriere plan sans l'arreter
int tichnas_bg(int pid) {
  if (kill(pid, SIGCONT) < 0) {
    perror("Error");
    return -1;
  }
printf("le processus pid tourne en arriere plan............\n");
  return 0;
}