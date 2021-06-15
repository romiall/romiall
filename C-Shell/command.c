#include "command.h"

#include "headers.h"

void getCommands(char* command, char*** commands) {
  char* str = NULL;
  long unsigned size = 1024;

  int len = getline(&str, &size, stdin);

  if (len < 0) {
    printf("\n");
    exit(0);
  }

  strcpy(command, str);

  str[len - 1] = '\0';

  if (str[0] == '\0') return;

  char* tempCommands[20];

  tempCommands[0] = NULL;

  char* token = strtok(str, ";");

  for (ll i = 0; token != NULL; i++) {
    tempCommands[i] = token;
    tempCommands[i + 1] = NULL;
    token = strtok(NULL, ";");
  }

  commands[0] = NULL;

  for (int i = 0; tempCommands[i] != NULL; i++) {
    commands[i + 1] = NULL;

    commands[i] = malloc(20 * sizeof(char*));
    commands[i][0] = NULL;

    char* token = strtok(tempCommands[i], "|");

    for (int j = 0; token != NULL; j++) {
      commands[i][j + 1] = NULL;
      commands[i][j] = token;
      token = strtok(NULL, "|");
    }
  }
}
