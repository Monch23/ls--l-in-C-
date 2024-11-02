#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

/*struct stat {
 dev_t st_dev; // Устройство
 ino_t st_ino; // Индексный узел inode
 mode_t st_mode; // Разрешения и тип файла
 nlink_t st_nlink; // Количество жестких ссылок на файл
 uid_t st_uid; // ID пользователя-владельца
 gid_t st_gid; // ID группы-владельца
 dev_t st_rdev; // Тип устройства (если это файл устройства)
 off_t st_size; // Общий размер в байтах
 unsigned long st_blksize; // Размер блока для ввода/вывода
 unsigned long st_blocks; // Количество выделенных блоков
 time_t st_atime; // Время последнего доступа
 time_t st_mtime; // Время последнего изменения содержимого файла
 time_t st_ctime; // Время последнего изменения состояния файла
} */

int main(int argc, const char *argv[]) {
        int fd = open(argv[1], O_WRONLY);

		  if (fd < 0) {
				perror("open()");
				return -1;
		  }

        int perms = 11;
        char *ftypePerms = (char*)calloc(perms, sizeof(char));

        struct stat metadata;
        fstat(fd, &metadata);

        struct passwd *usrInfo;
        usrInfo = getpwuid(metadata.st_uid);

        struct passwd *grpInfo;
        grpInfo = getpwuid(metadata.st_gid);

        mode_t tmpStat = metadata.st_mode;
        nlink_t linkCnt = metadata.st_nlink;
        off_t fsize = metadata.st_size;
		  char *laccess = ctime(&metadata.st_atime);
		  laccess[strlen(laccess) - 9] = '\0'; // remove year and seconds
		  laccess += 4; // remove weekday

        if (S_ISREG(tmpStat)) {
                ftypePerms[0] = '-';
        } else if (S_ISCHR(tmpStat )) {
                ftypePerms[0] = 'c';
        } else if (S_ISBLK(tmpStat)) {
                ftypePerms[0] = 'b';
        } else if (S_ISLNK(tmpStat)) {
                ftypePerms[0] = 'l';
        } else if (S_ISDIR(tmpStat)) {
                ftypePerms[0] = 'd';
        }

        // USER permissions
        if (tmpStat & S_IRUSR) {
                ftypePerms[1] = 'r';
        } else {
                ftypePerms[1] = '-';
        }
        if (tmpStat & S_IWUSR) {
                ftypePerms[2] = 'w';
        } else {
                ftypePerms[2] = '-';
        }
        if (tmpStat & S_IXUSR) {
                ftypePerms[3] = 'x';
        } else {
                ftypePerms[3] = '-';
        }

        // GROUP permissions
        if (tmpStat & S_IRGRP) {
                ftypePerms[4] = 'r';
        } else {
                ftypePerms[4] = '-';
        }
        if (tmpStat & S_IWGRP) {
                ftypePerms[5] = 'w';
        } else {
                ftypePerms[5] = '-';
        }
        if (tmpStat & S_IXGRP) {
                ftypePerms[6] = 'x';
        } else {
                ftypePerms[6] = '-';
        }

        // OTHER permissions
        if (tmpStat & S_IROTH) {
                ftypePerms[7] = 'r';
        } else {
                ftypePerms[7] = '-';
        }
        if (tmpStat & S_IWOTH) {
                ftypePerms[8] = 'w';
        } else {
                ftypePerms[8] = '-';
        }
        if (tmpStat & S_IXOTH) {
                ftypePerms[9] = 'x';
        } else {
                ftypePerms[9] = '-';
        }

        printf("%s %ld %s %s %ld ", ftypePerms, linkCnt, usrInfo->pw_name, grpInfo->pw_name, fsize);
		  printf("%s %s \n", laccess, argv[1]); 
        free(ftypePerms);

		  int cl = close(fd);
 
		  if (cl < 0) {
				perror("close()");
				return -1;
		  }
		
		  return 0;
}
