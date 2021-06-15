#include "env.h"

#include "headers.h"

int tichnas_setenv(char* name, char* value) {
  if (value == NULL) {
    if (setenv(name, "", 1) < 0) {
      perror("Error");
      return -1;
    }
  } else {
    if (setenv(name, value, 1) < 0) {
      perror("Error");
      return -1;
    }
  }

  return 0;
}

int tichnas_unsetenv(char* name) {
  if (unsetenv(name) < 0) {
    perror("Error");
    return -1;
  }

  return 0;
}