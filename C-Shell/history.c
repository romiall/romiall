#include "history.h"

#include "headers.h"

int getHistory(char* root, char** commands) {
  long unsigned size = 1024;
  char path[1024];
  int noOfCommands = 0;

  commands[0] = NULL;
  strcpy(path, root);
  strcat(path, "/.history");

  FILE* file = fopen(path, "r");

  if (file) {
    for (int i = 0; i < 20; i++) {
      commands[i + 1] = NULL;
      if (getline(&commands[i], &size, file) > 0) {
        noOfCommands++;
      } else {
        break;
      }
    }
    fclose(file);
  }

  return noOfCommands;
}

void addHistory(char* root, char* str) {
  int valid = 0;

  for (int i = 0; str[i] != '\0' && !valid; i++) {
    if (str[i] != ' ' && str[i] != '\t') valid = 1;
  }

  if (!valid) return;

  char* commands[21];
  long unsigned size = 1024;
  char path[1024];

  int noOfCommands = getHistory(root, commands);

  if (noOfCommands > 0 && !strcmp(str, commands[noOfCommands - 1])) return;

  strcpy(path, root);
  strcat(path, "/.history");

  FILE* file = fopen(path, "w");

  if (file) {
    if (noOfCommands && noOfCommands < 20) fprintf(file, "%s", commands[0]);

    for (int i = 1; i < noOfCommands; i++) fprintf(file, "%s", commands[i]);

    fprintf(file, "%s", str);

    fclose(file);
  }
}

int tichnas_history(char* root, char* limitStr) {
  int limit = 10;

  if (limitStr) sscanf(limitStr, "%d", &limit);

  char* commands[21];

  int noOfCommands = getHistory(root, commands);

  if (noOfCommands < limit) limit = noOfCommands;

  for (int i = noOfCommands - limit; i < noOfCommands; i++)
    printf("%s", commands[i]);

  return 0;
}