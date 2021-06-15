#include "bg.h"
#include "cd.h"
#include "command.h"
#include "echo.h"
#include "env.h"
#include "fg.h"
#include "headers.h"
#include "history.h"
#include "jobs.h"
#include "kjob.h"
#include "ls.h"
#include "pinfo.h"
#include "prompt.h"
#include "pwd.h"
#include "runner.h"

char hostname[1024];
char* user;
char* root;
char cwd[1024];
char prevwd[1024] = ".";
int bgPid[100] = {1};
char* bgCommand[100];
int fgRunning = 0;
int error = 0;

void addBg(int pid, char* command) {
  if (!pid) return;

  int pos = 99;

  while (!bgPid[pos]) pos--;

  pos++;

  bgPid[pos] = pid;
  bgCommand[pos] = malloc(strlen(command) + 1);
  strcpy(bgCommand[pos], command);
}

void removeBg(int pid) {
  if (!pid) return;

  for (int i = 1; i < 100; i++) {
    if (bgPid[i] == pid) {
      bgPid[i] = 0;
      free(bgCommand[i]);
      break;
    }
  }
}

void bgHandler() {
  int status;
  int pid = waitpid(-1, &status, WNOHANG);

  if (pid > 0) {
    int pos = 0;
    while (bgPid[pos] != pid) pos++;

    if (WIFSTOPPED(status)) return;

    fprintf(stderr, "%s PID %d stope %s\n", bgCommand[pos], bgPid[pos],
            WIFEXITED(status) ? "normalement" : "anormalement");

    printPrompt(&error, user, root, cwd, hostname);

    bgPid[pos] = 0;
    free(bgCommand[pos]);
  }
}

void ctrlCZhandler() {
  if (!fgRunning) {
    printf("\n");
    printPrompt(&error, user, root, cwd, hostname);
  } else {
    fgRunning = 0;
  }
}

void shellLoop() {
  char** commands[20];
  char* token;
  char* args[512];
  int noOfArgs;
  int background;
  char command[1024];
  int inputFd;
  int outputFd;
  int stdinClone;
  int stdoutClone;
  int ioMode;
  int subCommands;
  int pipeFds[2][2];

  while (1) {
    getcwd(cwd, 1024);

    printPrompt(&error, user, root, cwd, hostname);

    getCommands(command, commands);

    addHistory(root, command);

    for (ll i = 0; commands[i] != NULL; i++) {  // semicolon separated
      subCommands = 0;
      while (commands[i][subCommands] != NULL) subCommands++;

      for (ll j = 0; j < subCommands; j++) {  // pipe separated
        error = 0;

        while (commands[i][j][0] == ' ' || commands[i][j][0] == '\t')
          commands[i][j]++;

        if (commands[i][j][0] == '\0') continue;

        inputFd = 0;
        outputFd = 0;
        ioMode = -1;  // 0 = input, 1 = output with new file, 2 = output with
                      // append file

        if (subCommands > 1 && j < subCommands - 1) {
          if (pipe(pipeFds[j % 2]) < 0) {
            perror("Error");
            break;
          }
          outputFd = pipeFds[j % 2][1];
        }
        if (subCommands > 1 && j > 0) inputFd = pipeFds[1 - j % 2][0];

        noOfArgs = 0;
        background = 0;
        args[0] = NULL;

        token = strtok(commands[i][j], " \t");

        while (token) {
          if (ioMode > -1) {
            if (ioMode) {
              outputFd =
                  open(token,
                       O_WRONLY | O_CREAT | (ioMode == 1 ? O_TRUNC : O_APPEND),
                       0644);
            } else {
              inputFd = open(token, O_RDONLY);
            }

            ioMode = -1;
          } else if (!strcmp(token, "&")) {
            background = 1;
          } else if (!strcmp(token, "<")) {
            ioMode = 0;
          } else if (!strcmp(token, ">")) {
            ioMode = 1;
          } else if (!strcmp(token, ">>")) {
            ioMode = 2;
          } else if (token[0] != '\0') {
            args[noOfArgs++] = token;
            args[noOfArgs] = NULL;
          }

          token = strtok(NULL, " \t");
        }

        if (!noOfArgs) {
          printf("Error: No command\n");
          continue;
        }

        if (inputFd < 0 || outputFd < 0) {
          perror("Error");
          continue;
        }

        if (inputFd) {
          stdinClone = dup(0);
          dup2(inputFd, 0);
        }
        if (outputFd) {
          stdoutClone = dup(1);
          dup2(outputFd, 1);
        }

        if (!strcmp(args[0], "quit")) {
          exit(0);

        } else if (!strcmp(args[0], "echo")) {
          if (tichnas_echo(args) < 0) error = 1;

        } else if (!strcmp(args[0], "pwd")) {
          if (tichnas_pwd(cwd) < 0) error = 1;

        } else if (!strcmp(args[0], "cd")) {
          if (noOfArgs > 2) {
            error = 1;
            printf("Error: trop d'arguments\n");
          } else if (tichnas_cd(root, cwd, prevwd, args[1]) < 0)
            error = 1;

        } else if (!strcmp(args[0], "ls")) {
          if (tichnas_ls(root, args + 1) < 0) error = 1;

        } else if (!strcmp(args[0], "pinfo")) {
          if (tichnas_pinfo(root, args[1]) < 0) error = 1;

        } else if (!strcmp(args[0], "history")) {
          if (noOfArgs > 2) {
            error = 1;
            printf("Error: trop d'arguments\n");
          } else if (tichnas_history(root, args[1]) < 0)
            error = 1;

        } else if (!strcmp(args[0], "setenv")) {
          if (noOfArgs > 3) {
            error = 1;
            printf("Error: trop d'arguments\n");
          } else if (tichnas_setenv(args[1], args[2]) < 0)
            error = 1;

        } else if (!strcmp(args[0], "unsetenv")) {
          if (noOfArgs > 2) {
            error = 1;
            printf("Error: trop d'arguments\n");
          } else if (tichnas_unsetenv(args[1]) < 0)
            error = 1;

        } else if (!strcmp(args[0], "jobs")) {
          if (tichnas_jobs(bgPid, bgCommand) < 0) error = 1;

        } else if (!strcmp(args[0], "kjob")) {
          if (noOfArgs < 3) {
            error = 1;
            printf("Error: tres peu d'arguments\n");
          } else {
            int jobNo = 0;
            int sig = 0;
            sscanf(args[1], "%d", &jobNo);
            sscanf(args[2], "%d", &sig);

            if (jobNo < 1 || jobNo > 99 || bgPid[jobNo] <= 1) {
              error = 1;
              printf("Error:processus invalide\n");
            } else if (tichnas_kjob(bgPid[jobNo], sig) < 0)
              error = 1;
          }
        } else if (!strcmp(args[0], "fg")) {
          if (noOfArgs < 2) {
            error = 1;
            printf("Error: tres peu d'arguments\n");
          } else {
            int jobNo = 0;
            sscanf(args[1], "%d", &jobNo);

            if (jobNo < 1 || jobNo > 99 || bgPid[jobNo] <= 1) {
              printf("Error:processus invalide\n");
            } else {
              int pid = tichnas_fg(bgPid[jobNo]);

              removeBg(pid > 0 ? pid : -pid);

              if (pid <= 0) error = 1;
            }
          }

        } else if (!strcmp(args[0], "overkill")) {
          for (int k = 1; k < 100; k++) {
            if (bgPid[k]) {
              if (tichnas_kjob(bgPid[k], SIGKILL) < 0) error = 1;
            }
          }

        } else if (!strcmp(args[0], "bg")) {
          if (noOfArgs < 2) {
            error = 1;
            printf("Error: tres peu d'arguments\n");
          } else {
            int jobNo = 0;
            sscanf(args[1], "%d", &jobNo);

            if (jobNo < 1 || jobNo > 99 || bgPid[jobNo] <= 1) {
              printf("Error:processus invalide\n");
            } else {
              if (tichnas_bg(bgPid[jobNo]) < 0) error = 1;
            }
          }
        } else if (!strcmp(args[0], "lol")) {
        } else {
          fgRunning = 1;

          int pid = runCommand(background, args);

          if (pid < 0) {
            error = 1;
          } else if (pid > 0) {
            addBg(pid, args[0]);
            if (!background) error = 1;
          }

          fgRunning = 0;
        }

        if (inputFd) {
          close(inputFd);
          dup2(stdinClone, 0);
          close(stdinClone);
        }
        if (outputFd) {
          close(outputFd);
          dup2(stdoutClone, 1);
          close(stdoutClone);
        }

        if (subCommands > 1 && j < subCommands - 1) close(pipeFds[j % 2][1]);
        if (subCommands > 1 && j > 0) close(pipeFds[1 - j % 2][0]);
      }

      commands[i] = NULL;
    }
  }
}

int main() {
  root = getenv("PWD");
  user = getenv("USER");
  gethostname(hostname, 1024);
 printf("\n------------------------------------------------------------------------------------");
  printf("\n------------------------------------------------------------------------------------\n");
  printf("\t\t[TP] [SHELL PERSONALISE] [INFO 312]\n");

  signal(SIGCHLD, bgHandler);
  signal(SIGINT, ctrlCZhandler);
  signal(SIGTSTP, ctrlCZhandler);

  shellLoop();

  return 0;
}
