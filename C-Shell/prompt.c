#include "prompt.h"

#include "headers.h"

void printPrompt(int* error, char* user, char* root, char* cwd,
                 char* hostname) {
  char path[1024] = " ";

  for (ll i = 0;; i++) {
    if (root[i] == '\0') {
      path[0] = '~';
      break;
    } else if (cwd[i] == '\0' || cwd[i] != root[i]) {
      break;
    }
  }

  if (path[0] == '~') {
    strcat(path, cwd + strlen(root));
  } else {
    strcpy(path, cwd);
  }

  printf("%s<\033[01;32m%s@%s\033[01;00m:\033[01;34m%s\033[01;00m>",
         *error ? "\033[01;31m:'(\033[01;00m" : "\033[01;34m:')\033[01;00m",
         user, hostname, path);

  *error = 0;

  fflush(stdout);
}
