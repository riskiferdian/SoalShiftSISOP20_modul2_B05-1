# SoalShiftSISOP20_modul2_B05

## 1. Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
### 1a.Program menerima 4 argumen berupa:i. Detik: 0-59 atau * (any value) ii. Menit: 0-59 atau * (any value) iii. Jam: 0-23 atau * (any value) iv. Path file .sh
```
sec = atoi(argv[1]);
min = atoi(argv[2]);
hour = atoi(argv[3]);
strcpy(cmd, argv[4]);
```
Untuk menerima argumen, kami menggunakan argc dan argv. argv adalah argument vector yang akan menyimpan argument ke dalam array dan argc adalah argument count yang akan menyimpan berapa banyak argument yang diinputkan. Ketika memberikan argument misal ./Soal1 \1 40 19 /home/afiahana/Documents/hello.sh maka
- ./Soal1 adalah argv[0]
- 1 adalah argv[1] : menerima argument untuk detik
- 40 adalah argv[2] : menerima argument untuk menit
- 19 adalah argv[3] : menerima argument untuk jam
- /home/afiahana/Documents/hello.sh adalah argv[4] : menerima argument untuk path file .sh
Kami menggunakan atoi, karena melalui argv, argument yang disimpan masih dalam bentuk string. Sedangkan detik, menit, dan jam seharusnya berupa integer. Maka dari itu dengan function atoi, mengubah string menjadi integer.

### 1b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
```
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
```
Langkah-langkah:
- Program menerima 4 argument
``` if(argc < 5 || argc > 5) printf("Salah Argumen\n"); ```
- Detik: 0-59 atau * (any value)
```
if(strcmp(argv[1], "*") != 0){
  if(sec < 0 || sec > 59) printf("Input detik yang benar hanya 0-59 atau *\n");
}
```
- Menit: 0-59 atau * (any value)
```
if(strcmp(argv[2], "*") != 0){
	if(min < 0 || min > 59) printf("Input menit yang benar hanya 0-59 atau *\n");
}
```
- Jam: 0-23 atau * (any value)
```
if(strcmp(argv[3], "*") != 0){
	if(hour < 0 || hour > 23) printf("Input jam yang benar hanya 0-23 atau *\n");
}
```
- Path file .sh
```
char* ext;
ext = strchr(cmd,'.');
if(strcmp(ext+1, "s") != 0 && strcmp(ext+2, "h") != 0 && strlen(ext) != 2) printf("File harus .sh\n");
```
Disini kami menggunakan ```strchr``` yang digunakan untuk mencari kemunculan terakhir dari sebuah karakter. Kami menggunakannya untuk mencari kemunculan terakhir dari karakter . karena setelah . adalah file extension. Soal hanya menerima jika .sh . Maka dari itu kami cek, apakah setelah . adalah sh atau bukan. Selain itu cek apakah setelah . hanya ada 2 karakter. Karena bisa saja .sh123 (misal). Jika tidak mengecek banyak karakter setelah . bisa saja extension tersebut dianggap valid.

### 1c. Program hanya menerima 1 config cron
``` if(argc < 5 || argc > 5) printf("Salah Argumen\n"); ```
Program hanya menerima 4 argumen dan dari soal 1b sudah dikasi ketentuan. Sehingga program hanya menerima 1 config cron

### 1d. Program berjalan di background (daemon)
```
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
```
Untuk membuat daemon, saya ambil dari template di Modul 2
Langkah-langkah:
- Mengecek waktu sekarang
```
time_t rawtime;
time(&rawtime);
struct tm *info = localtime(&rawtime);
	
curr_hour = info->tm_hour;
curr_min = info->tm_min;
curr_sec = info->tm_sec;
```
Kami menggunakan <time.h> lalu mengambil jam, menit, detik.
- Mengecek apakah config cron sama dengan waktu sekarang
``` if((sec == curr_sec || flag_s == 1) && (min == curr_min || flag_m == 1) && (hour == curr_hour || flag_h == 1)) ```
- fork() dan exec() supaya ketika sudah menjalankan program tidak mati
- Menaruh exec di child process dan menaruh wait di parent agar program tidak mati setelah dijalankan
- Memberi sleep(1) agar mengecek tiap saat
```
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
```

### 1e. Tidak boleh menggunakan fungsi system()
Bisa cek di Source Code

## 2. Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat sebuah program.
### 2a. Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].

```
pid_t cid, cid2, cid3, cid4;
time_t t1 = time(NULL);
struct tm* p1 = localtime(&t1);
strftime(curtime, 30, "%Y-%m-%d_%H:%M:%S", p1);

cid = fork();
if(cid < 0) exit(0);
if(cid == 0)
{
    char *ag[] = {"mkdir", curtime, NULL};
    execv("/bin/mkdir", ag);
}
sleep(30); 
```
Untuk membuat folder dengan timestamp [YYYY-mm-dd_HH:ii:ss] dapat menggunakan fungsi strftime dari C dan menggunakan fork serta exec untuk memanggil command mkdir. Agar program berjalan setiap 30 detik, maka digunakan daemon dan diberi selang 30 detik dengan sleep(30).

### 2b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY- mm-dd_HH:ii:ss].

``` 
chdir(curtime);
for(i = 0; i < 20; i++)
{
    time_t t2 = time(NULL);
    struct tm* p2 = localtime(&t2);
    strftime(curtime2, 30, "%Y-%m-%d_%H:%M:%S", p2);
    sprintf(link, "https://picsum.photos/%ld", (t2 % 1000) + 100);

    cid3 = fork();
    if(cid3 < 0) exit(0);
    if(cid3 == 0)
    {
        char *ag[] = {"wget", link, "-O", curtime2, "-o", "/dev/null", NULL};
        execv("/usr/bin/wget", ag);
    }
    sleep(5);
}
```

Untuk mendownload gambar untuk tiap folder, digunakan fungsi fork dan exec untuk memanggil fungsi wget, dengan format untuk mendapatkan timestamp sama seperti di soal 2a. Sebelum menjalankan perintah tersebut, akan memanggil fungsi chdir(curtime) terlebih dahulu, agar dapat masuk ke folder tersebut. Karena membutuhkan download 20 gambar, maka digunakan loop for dengan iterasi 20 kali. Berikut kodenya:

### 2c. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
```
while(wait(&stat2) > 0);
chdir("..");
strcpy(curtime3, curtime);
strcat(curtime3, ".zip");

cid4 = fork();
if(cid4 < 0) exit(0);
if(cid4 == 0)
{
    char *ag[] = {"zip", "-r", curtime3, curtime, NULL};
    execv("/usr/bin/zip", ag);
}
while(wait(&stat3) > 0);

char *ag[] = {"rm", "-r", curtime, NULL};
execv("/bin/rm", ag);

```
Setelah keluar dari loop (sudah mendownload 20 gambar), maka perlu melakukan zip folder tadi dengan nama yang sama yaitu nama folder tadi (curtime). Untuk mendapatkan nama folder, digunakan fungsi strcpy(curtime3, curtime) yang berarti variable curtime3 akan berisi sama dengan curtime (nama folder), kemudian dilanjutkan dengan fungsi strcat(curtime3, ".zip") agar menambahkan ekstensi .zip. Untuk memanggil fungsi zip, harus menggunakan fork dan exec lagi. Sebelumnya, maka harus berpindah ke direktori parent (keluar), karena akan dilakukan zip, dengan command chdir(".."). Dan kemudian command rm -r agar folder awalnya terhapus.

### 2d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men- generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
```
FILE* kill;
kill = fopen("kill.sh", "w");
fprintf(kill, "#!/bin/bash\nkill %d\nkill %d\necho \'Killed program.\'\nrm \"$0\"", getpid() + 2, getpid() + 3);
fclose(kill);
pid_t cid;
cid = fork();
if(cid < 0) exit(0);
if(cid = 0)
{
    char *ag[] = {"chmod", "u+x", "kill.sh", NULL};
    execv("/bin/chmod", ag);
}
while(wait(&stat) > 0);
```
Program killer yang kami buat dalam bentuk bash, di mana dituliskan dari program C dengan command fopen terlebih dahulu untuk membuat file baru yang akan dituliskan, dengan command w, dan command fprintf untuk menuliskan source code killer. Kemudian memanggil fungsi exec dan fork lagi untuk mengganti hak akses file tersebut menggunakan command chmod u+x kill.sh, yang berarti user dapat mengeksekusi file kill.sh tersebut.

### 2e. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
```
int main(int argc, char* argv[])
{
    if(argc != 2 || (argv[1][1] != 'a' && argv[1][1] != 'b')) 
    {
        printf("Mode hanya ada -a atau -b\n");
        exit(0);
    }
    ...
    while(1)
    {
        ...
        if(argv[1][1] == 'a') prctl(PR_SET_PDEATHSIG, SIGHUP);
        ...
    }
    ...
}
```

Untuk menerima argumen, digunakan parameter di fungsi main, dalam bentuk argc (untuk mengetahui berapa banyak argumen), dan array argv yang berisi argumen yang dipassing. Jika argumen yang diinginkan berupa -a dan -b, artinya huruf a dan b berada pada array argv[1][1] karena di argumen kedua dan karakter kedua (zero indexing). Kemudian untuk membedakan mode a, di mana program langsung berhenti tanpa menyelesaikan pekerjaannya dulu, maka diberi command if di dalam loop pembuatan folder dalam while(1), agar jika mode yang dipassing adalah a, dapat langsung memberikan sinyal hangup dengan SIGHUP dengan bantuan fungsi prctl. 

## 3. Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan (multiprocessing).
### 3a. Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
```
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
```
Langkah-langkah :
- fork() dua kali, karena nanti akan digunakan untuk sub-soal selanjutnya
- Di proses pertama (parent process) akan membuat directory 'indomie' lalu sleep 5 detik
- Di proses kedua (child1 process) akan membuat directory 'sedaap'

### 3b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.
```
else if(child_id1 > 0 && child_id2 == 0){
    //child2
    char *argv3[] = {"unzip", "/home/afiahana/Sisop/Modul2/Soal3/jpg.zip", NULL};
    execv("/usr/bin/unzip", argv3);
  }
```
Di proses ketiga (child2 process) akan unzip file 'jpg.zip'

### 3c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.

### 3d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
