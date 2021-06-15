#include "cd.h"

#include "headers.h"

int tichnas_cd(char* root, char* cwd, char* prevwd, char* newdir) {
  char newwd[1024] = "";

  if (newdir == NULL) {
    sprintf(newwd, "%s", root);

  } else if (!strcmp(newdir, "-")) {
    if (prevwd[0] == '.') {
      printf("%s\n", cwd);
      return 0;
    }

    int isRelative = 0;

    if (chdir(prevwd) < 0) {
      perror("Error");
      return -1;
    }

    for (int i = 0;; i++) {
      if (root[i] == '\0') {
        isRelative = 1;
        break;
      }
      if (prevwd[i] == '\0' || prevwd[i] != root[i]) break;
    }

    if (isRelative) {
      prevwd += strlen(root) - 1;
      prevwd[0] = '~';
    }

    printf("%s\n", prevwd);

    return 0;

  } else if (newdir[0] == '/') {
    sprintf(newwd, "%s", newdir);

  } else {
    strcat(newwd, newdir[0] == '~' ? root : cwd);

    if (newdir[0] != '~') strcat(newwd, "/");

    strcat(newwd, newdir + (newdir[0] == '~'));
  }

  strcpy(prevwd, cwd);

  if (chdir(newwd) < 0) {
    perror("Error");
    return -1;
  }

  return 0;
}
