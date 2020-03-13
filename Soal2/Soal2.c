#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>

int main() {
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) {
    pid_t child_id;
    int status;

    child_id = fork();
  
    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
        char arrpath[20] = "/home/afiahana/";
        char buffer[25];

        time_t rawtime;
        time(&rawtime);
        struct tm *info = localtime(&rawtime);

        strftime(buffer, 25, "%d-%m-%G_%H:%M:%S", info);
        char *foldername = strcat(arrpath, buffer);

        char *argv[] = {"mkdir", "-p", foldername, NULL};
        execv("/bin/mkdir", argv);
    } 
    
    else {
        // this is parent
        while ((wait(&status)) > 0);

        int i;

        for(i = 0; i < 20; i++){
            char sftime[25];

            time_t rawtime;
            time(&rawtime);
            struct tm *info = localtime(&rawtime);

            strftime(sftime, 25, "%d-%m-%G_%H:%M:%S", info);
            char *filename = sftime;

            char *argv1[] = {"wget", "https://picsum.photos/200", "-O", filename};
            execv("/usr/bin/wget", argv1);
            sleep(5);
        }
        
    }

    sleep(30);
  }
}
