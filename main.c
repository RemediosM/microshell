/* Schemat microshella zaczerpnięty z podręcznika:
 * Tanenbaum, A., & Bos, H. (2016). Wywołania systemowe do zarządzania procesami.
 * W: Systemy operacyjne (pp. 79-82, wyd. 4). Gliwice: Helion. */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_MAGENTA "\033[1;35m"
#define CLEAR_FORMAT "\033[0m"
#define TRUE 1

void type_login();

void type_prompt();

void help();

int my_cd(char *cmd);

int my_cd_no_args();

char *cmd[1];

void read_command(char *cmd[]);

int my_exit(int status);

int my_exit_no_args();

void print_error();

int main() {

    cmd[0] = NULL;
    int status;
    while(TRUE){
        type_login();
        type_prompt();
        read_command(cmd);

        if(strcmp(cmd[0], "help") ==0 ){
            help();
        } else if (strcmp(cmd[0], "cd") ==0) {
            if(cmd[1] != NULL) {
                if (my_cd(cmd[1]) == -1) {
                    print_error();                }
            } else {
                if (my_cd_no_args() == -1) {
                    print_error();
                }
            }
        } else if (strcmp(cmd[0], "exit") == 0){
            if(cmd[1] != NULL) {
                my_exit(atoi(cmd[1]));
            }else {
                my_exit_no_args();
            }
        } else {
            if (fork() != 0) {
                waitpid(-1, &status, 0);
            } else {
                if (execvp(cmd[0], cmd) == -1) {
                    print_error();
                    exit(0);}
            }
        }

    }
}

void read_command(char *cmd[])
{
    char tmp[256];
    int n = 0;
    fgets(tmp,sizeof(tmp), stdin);
    char * p = strchr(tmp, '\n');
    *p = 0;
    cmd[n++] = strtok (tmp, " ");
    while (cmd[n - 1] != NULL)
        cmd[n++] = strtok (NULL, " ");
}

void type_login(){

    char *login = getlogin();
    printf(BOLD_GREEN "%s" "@", login);
    printf(CLEAR_FORMAT);
}

void type_prompt() {

    char *path = getcwd(NULL,0);
    printf(BOLD_CYAN "%s", path);
    printf(BOLD_MAGENTA ":~$ ");
    printf(CLEAR_FORMAT);

}

void help() {

    printf(BOLD_GREEN);
    printf("+----------------------------------+\n");
    printf(BOLD_MAGENTA);
    printf("| Moje funkcje to:                 |\n");
    printf("| * cd                             |\n");
    printf("| * exit                           |\n");
    printf("| * help                           |\n");
    printf(BOLD_GREEN);
    printf("+----------------------------------+\n");
    printf(CLEAR_FORMAT);

}

int my_cd(char *cmd ){

    if(strcmp(cmd,"~") == 0){

        char *home = getenv("HOME");
        if(chdir(home)== -1){
            return -1;
        }
    } else {
        if(chdir(cmd) == -1 ){
            return -1;
        }
    }
    return 0;
}

int my_cd_no_args() {

    char *home = getenv("HOME");

    if(chdir(home)== -1){
        return -1;
    }
    return 0;
}

int my_exit(int status){

        exit(status);
}

int my_exit_no_args(){
    exit(0);
}

void print_error(){
    printf(BOLD_RED "%s" CLEAR_FORMAT "\n", strerror(errno));

}