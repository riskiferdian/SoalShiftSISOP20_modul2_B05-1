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

int main(int argc, char *argv[]) {

	int hour, min, sec, curr_hour, curr_min, curr_sec, flag_s, flag_m, flag_h;
	char cmd[100];

	sec = atoi(argv[1]);
	min = atoi(argv[2]);
	hour = atoi(argv[3]);
	strcpy(cmd, argv[4]);

	if(argc < 5 || argc > 5) printf("Salah Argumen\n");

	if(strcmp(argv[1], "*") != 0){
		if(sec < 0 || sec > 59) printf("Input detik yang benar hanya 0-59 atau *\n");
	}
	
	if(strcmp(argv[2], "*") != 0){
		if(min < 0 || min > 59) printf("Input menit yang benar hanya 0-59 atau *\n");
	}
	
	if(strcmp(argv[3], "*") != 0){
		if(hour < 0 || hour > 23) printf("Input jam yang benar hanya 0-23 atau *\n");
	}
	
	char* ext;
	ext = strchr(cmd,'.');
	if(strcmp(ext+1, "s") != 0 && strcmp(ext+2, "h") != 0 && strlen(ext) != 2) printf("File harus .sh\n");

	if(strcmp(argv[1], "*") == 0) flag_s = 1;
	if(strcmp(argv[2], "*") == 0) flag_m = 1;
	if(strcmp(argv[3], "*") == 0) flag_h = 1;

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


  	while (1) {
		time_t rawtime;
		time(&rawtime);
		struct tm *info = localtime(&rawtime);
	
		curr_hour = info->tm_hour;
		curr_min = info->tm_min;
		curr_sec = info->tm_sec;

		if((sec == curr_sec || flag_s == 1) && (min == curr_min || flag_m == 1) && (hour == curr_hour || flag_h == 1)){
			pid_t child_id;
			int status;
			child_id = fork();

			if(child_id < 0){
				exit(EXIT_FAILURE);
			}

			if(child_id == 0){
				char *proses[] = {"bash", cmd, NULL};
				execv("/bin/bash", proses);
			}

			else{
				while((wait(&status)) > 0);
			}
		}
		sleep(1);
  	}
}
