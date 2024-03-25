#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <string.h>

#include <errno.h>

#include <pwd.h>



#include "ls.c"

#include "mkdir.c"

#include "cat.c"

#include "rm.c"

#include "date.c"



void pwd();

void cd(char parsed[1000][1000]);

int parser(char input[1000], char parsed[1000][1000]);

void echo(char input[1000]);

void history(char parsed[1000][1000]);

void whoami();

void help();



FILE *fptr;



int main() {

    printf("<<<WELCOME TO OUR TERMINAL>>>\n");

    printf("!!!USE AT YOUR OWN RISK!!!\n\n");

    printf("Enter 'help' command to list all the available commands.\n");



    while(1) {

        char cwd[256] = {};

        getcwd(cwd, 256);

        printf("myterminal@OSEL~%s $ ", cwd);



        char input[1000] = {};

        char parsed[1000][1000] = {{}};



        FILE *ptr;

        ptr = fopen("/home/karthik/Desktop/history.txt","a");



        fgets(input, 1000, stdin);



        int com = parser(input, parsed);



        fputs(input, ptr);

        fclose(ptr);



        int i;

        switch (com) {

            case 0:

                printf("Command not found.\n");

                break;

            case 1:

                cd(parsed);

                break;

            case 2:

                cat(parsed);

                break;

            case 3:

                echo(input);

                break;

            case 4:

                i = 8;

                int flag = 0;

                while (input[i] != '\0' && input[i] != '\n') {

                    if (input[i] == 'c') {

                        flag = 1;

                        break;

                    } else if (input[i] == 'd') {

                        flag = 2;

                        break;

                    } else;

                    ++i;

                }

                if (flag == 0)

                    history(parsed);

                else if (flag == 1) {

                    remove("/home/karthik/Desktop/history.txt");

                    fopen("/home/karthik/Desktop/history.txt","w");

                }

                break;

            case 5:

                pwd();

                break;

            case 6:

                exit(0);

                break;

            case 7:

                rm(parsed);

                break;

            case 8:

                ls1(input);

                break;

            case 9:

                mkdir(parsed);

                break;

            case 10:

                date(parsed);

                break;

            case 11:

                whoami();

                break;    

            case 12:

                help();

                break;

            case 13:

                grep(parsed); 

                break;        

            default:

                printf("Command not found.\n");

                break;

        }

    }

}



void history(char parsed[1000][1000]) {

    int i = 0;

    int flag = 0;

    char n;

    while (i < 1000 && *parsed[i] != '\n') {

        if (flag == 0 && strcmp(parsed[i], "history") == 0) {

            flag = 1;

        }

        ++i;

    }

    if (flag == 1) {

        fptr = fopen("/home/karthik/Desktop/history.txt","r");

        char h = fgetc(fptr);

        while (h != EOF) {

            printf("%c", h);

            h = fgetc(fptr);

        }

        fclose(fptr);

    }

}



void echo(char input[1000]) {

    int i = 0;

    while (input[i] != '\0' || input[i] != '\n') {

        if (input[i] == 'e' && input[i+1] == 'c' && input[i+2] == 'h' && input[i+3] == 'o') break;

        ++i;

    }



    int j = i + 5;

    int flag = 0;

    while (j < 1000 && (input[j] != '\0' && input[j] != '\n')) {

        if (flag == 0 && input[j] == '"') break;

        else if (flag == 0 && input[j] == '-') {

            flag = 1;

        } else if (flag == 0 && input[j] == '*') {

            flag = 4;

        }   

        else if (flag == 1 && input[j] == 'n') {

            flag = 2;

            break;

        } else if (flag == 1 && input[j] == 'e') {

            flag = 3;

            break;

        } else;

        ++j;

    }

    char string[256] = {};

    int o = 0;

    if (flag == 0) {

        while (j < 1000 && o < 256 && (input[j] != '\0' && input[j] != '\n')) {

            if (input[j] != '"' && input[j] != '\n') {

                string[o] = input[j];

                ++o;

            }

            ++j;

        }

        printf("%s\n", string);

    }

    if (flag == 2) {

        int first = 0;

        j = j + 1;

        while (j < 1000 && o < 256 && (input[j] != '\0' || input[j] != '\n')) {

            if (input[j] == '"') first = 1;

            if (first == 1 && input[j] != '"' && input[j] != '\n') {

                string[o] = input[j];

                ++o;

            }

            ++j;

        }

        printf("%s", string);

    }

    if (flag == 4) {

        pid_t pid;

        pid = fork();

        if (pid < 0) {

            printf("Some error occurred.\n");

        } else if (pid == 0) {

            execl("/bin/ls", "ls", (char *)0);

        } else {

            wait(NULL);

        }

    }

}



void pwd() {

    char cwd[256];

    if (*getcwd(cwd, 256) != '\0') {

        printf("Current working directory: %s\n", cwd);

    } else {

        printf("Error in pwd.\n");

    }

}



void cd(char parsed[1000][1000]) {

    int i = 0;

    while (i < 1000 && *parsed[i] != '\n') {

        if (strcmp(parsed[i], "cd") != 0) {

            break;

        }

        ++i;

    }

    if (parsed[i][0] == '\0' || parsed[i][0] == '~') {

        chdir("/home/karthik");

    } else {

        chdir(parsed[i]);

    }

}



void whoami() {

    struct passwd *pw;

    uid_t uid;

    uid_t NO_UID = -1;

    uid = geteuid();

    pw = uid == NO_UID && errno ? NULL : getpwuid(uid);

    if (!pw) {

        fprintf(stderr, "Cannot find name for user ID %lu\n", (unsigned long int) uid);

        exit(EXIT_FAILURE);

    }

    printf("%s\n", pw->pw_name);

}

void help() {

    printf("Available commands:\n");

    printf("1. cd\n");

    printf("2. cat\n");

    printf("3. echo\n");

    printf("4. history\n");

    printf("5. pwd\n");

    printf("6. exit\n");

    printf("7. rm\n");

    printf("8. ls\n");

    printf("9. mkdir\n");

    printf("10. date\n");

    printf("11. whoami\n");

    printf("12. help\n");

}



int parser(char input[1000], char parsed[1000][1000]) {

    int i = 0;

    int j = 0;

    int o = 0;

    for (i = 0; i < 1000; i++) {

        if (input[i] == ' ' || input[i] == '\n') {

            ++j;

            o = 0;

        } else {

            parsed[j][o] = input[i];

            ++o;

        }

    }

    i = 0;

    j = 0;

    if (*parsed[0] == '\n') return 0;

    while (i < 1000 && *parsed[i] != '\n') {

        if (strcmp(parsed[i], "whoami") == 0) {

            j = 11;

            break;

        }

        if (strcmp(parsed[i], "cat") == 0) {

            j = 2;

            break;

        }

        if (strcmp(parsed[i], "echo") == 0) {

            j = 3;

            break;

        }

        if (strcmp(parsed[i], "history") == 0) {

            j = 4;

            break;

        }

        if (strcmp(parsed[i], "pwd") == 0) {

            j = 5;

            break;

        }

        if (strcmp(parsed[i], "exit") == 0) {

            j = 6;

            break;

        }

        if (strcmp(parsed[i], "rm") == 0) {

            j = 7;

            break;

        }

        if (strcmp(parsed[i], "ls") == 0) {

            j = 8;

            break;

        }

        if (strcmp(parsed[i], "mkdir") == 0) {

            j = 9;

            break;

        }

        if (strcmp(parsed[i], "date") == 0) {

            j = 10;

            break;

        }

        if (strcmp(parsed[i], "cd") == 0) {

            j = 1;

            break;

        }

        if (strcmp(parsed[i], "help") == 0) {

            j = 12;

            break;

        }

        if (strcmp(parsed[i], "grep") == 0) {

            j = 13;

            break;

        }

        ++i;

    }

    return j;
}

