## <b><H1>UNIVERSITE DE YAOUNDE I </B>
## <b><H1>TP INFO 312: </B>
## Personalisation du C Shell

##  PEGHIEMBOUO ROMIAL CARLOS

---

## Dependances;'

- GCC compiler
- Linux OS (de preference Ubuntu)

## Execution du Shell

Depuis la racine du projet executer la commande: `make` suivie de `./main`

## Fichiers et functions

### Fichiers supportes

###### Files containing functions required for proper functioning of the shell.

- `main.c`

  - Execute le Shell en boucle
  - Extrait des argument des commandes
  - Lie les differentes fonctions du programme

- `prompt.c`

  - Renvoit le prompt
  - Recherche les processus fils termines

- `command.c`

  - Recupere l'entree de l'utilisateur
  - Divise l'entree en de differentes commandes

- `runner.c`

  - Execute le commande aussi bien en foreground qu'en background

- `history.c`

  - ajoute une commande a l'historique des commandes
  - Le nombre maximmum de caractere est de 20

### Les fichiers de commande

###### fichiers contenant des function pour executer des commandes 

- `env.c`

  - Contient les commandes  `setenv` et `unsetenv`.
  - utilise pour configurer les variables d'environnement

- `jobs.c`

  - Affiche tous les processus courrants.

- `kjob.c`

  - Utilise pour envoyer une interruption a un processus en utilisant son PID.
  - Egalement employe en boucle pour implementer la commande `overkill`.

- `fg.c`

  - Ramene un processus en avant plan .

- `bg.c`

  - Reactive un processus qui etait stope en arriere plan.

- `pwd.c`

  - Affiche le repertoire courant

- `echo.c`

  - Affiche un message a l'utilisateur

- `cd.c`

  - Change de repertoire

- `ls.c`

  - Affiche des dossiers et de sous-dossiers du repertoire courant
 

- `pinfo.c`

  - Affiche les informations du processus dont le PID est passe en parametre et le processus courant si rien n'est specifie en parametre

- `history.c`

  - Affiche les `n` dernieres commande ou `n` est un nombre specifie en parametre par l'utilisateur et 10 si rien n'est specifier.

### autres Implementations

###### Commands/functions qui sont dans le meme fichier.

- Signals

  - `Ctrl+Z` envoie le processus courant en background
  - `Ctrl+C` Stops le processus en foreground courant

- I/O Redirection et Pipes

  - le parsing est fait dans  `command.c`.
  - `main.c` controle toutes les alternatives.

- Manipulations  Background  des Processus
  - implementation du listing des processus en bacground, leur ajout/suppression a/de la liste est faite dans `main.c`
- Exit Codes

> Projet  a continuer!
