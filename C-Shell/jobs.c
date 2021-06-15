#include "jobs.h"

#include "headers.h"

int tichnas_jobs(int* bgPid, char** bgCommand) {
  char fileName[1024];
  char status;
  int retValue = 0;

  for (int i = 1; i < 100; i++) {
    if (!bgPid[i]) continue;

    sprintf(fileName, "/proc/%d/stat", bgPid[i]);

    FILE* file = fopen(fileName, "r");

    if (file) {
      fscanf(file, "%*d %*s %c", &status);

      printf("[%d] %s %s [%d]\n", i, status == 'T' ? "Stopped" : "Running",
             bgCommand[i], bgPid[i]);

    } else {
      perror("Error");
      retValue = -1;
    }

    fclose(file);
  }

  return retValue;
}