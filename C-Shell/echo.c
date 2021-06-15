#include "echo.h"

#include "headers.h"

int tichnas_echo(char** args) {
  int i = 1;

  while (args[i + 1] != NULL) {
    printf("%s ", args[i]);
    i++;
  }

  if (args[i] != NULL) printf("%s", args[i]);
  printf("\n");

  return 0;
}