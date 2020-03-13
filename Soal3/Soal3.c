#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t child_id1, child_id2;
  int status;

  child_id1 = fork();
  child_id2 = fork();

  if (child_id1 < 0 || child_id2 < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if(child_id1 > 0 && child_id2 > 0){
    //parent
    char *argv1[] = {"mkdir", "-p", "indomie", NULL};
    execv("/bin/mkdir", argv1);
    sleep(5);
  }

  else if(child_id1 == 0 && child_id2 > 0){
    //child1
    char *argv2[] = {"mkdir", "-p", "sedaap", NULL};
    execv("/bin/mkdir", argv2);
  }

  else if(child_id1 > 0 && child_id2 == 0){
    //child2
    char *argv3[] = {"unzip", "/home/afiahana/Sisop/Modul2/Soal3/jpg.zip", NULL};
    execv("/usr/bin/unzip", argv3);
  }

  else{
    //child3
  }
}
