//#include "stdio.h"
//#include "stdlib.h"
#include "malloc.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "dirent.h"
#include "skiFile.h"

int getFileStats(int fd, char* type, off_t* size)
{
	struct stat sb;
	if(fstat(fd, &sb) < 0){
		goto FILE_STAT_FAILED;
	}

	switch(sb.st_mode & S_IFMT){
		case S_IFREG:
			*type = 'r';
			*size = sb.st_size;
			break;
		case S_IFDIR:
			*type = 'd';
			*size = sb.st_nlink;
			break;
		case S_IFLNK:
			*type = 'l';
			*size = sb.st_size;
			break;
		case S_IFSOCK:
			*type = 's';
			*size = 0;
			break;
		case S_IFCHR:
			*type = 'c';
			*size = 0;
			break;
		case S_IFBLK:
			*type = 'b';
			*size = 0;
			break;
		case S_IFIFO:
			*type = 'f';
			*size = 0;
			break;
		default:
			*type = 'x';
			*size = 0;
			break;
	}

	return 0;
FILE_STAT_FAILED:
	return -1;
}

int readFile(char* filename, DataFunc datafunc, void* output)
{
	int ret = -1;
	char type = 'x';
	off_t size = 0;
	int fd = -1;
	char* buffer = NULL;
	off_t readSize = 0;

	if((fd = open(filename, O_RDONLY)) < 0){
		goto FILE_OPEN_FAILED;
	}

	if(getFileStats(fd, &type, &size) < 0){
		goto FILE_GET_STAT_FAILED;
	}

	if(type != 'r' || type != 'c' || type != 'f'){
		goto FILE_TYPE_FAILED;
	}

	if((buffer = malloc(FILE_READ_BLOCK)) == NULL){
		goto BUFFER_MALLOC_FAILED;
	}

	while((readSize = read(fd, buffer, FILE_READ_BLOCK)) >= 0){
		if(datafunc(readSize, buffer, output))break;
	}

	ret = 0;

	free(buffer);
	buffer = NULL;
BUFFER_MALLOC_FAILED:
FILE_TYPE_FAILED:
FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return ret;
}

int readFileAll(char* filename, off_t apSize, DataFunc datafunc, void* output)
{
	int ret = -1;
	char type = 'x';
	off_t size = 0;
	int fd = -1;
	char* buffer = NULL;
	off_t length = 0;

	if((fd = open(filename, O_RDWR|O_CREAT, 0640)) < 0){
		goto FILE_OPEN_FAILED;
	}

	if(getFileStats(fd, &type, &size) < 0){
		goto FILE_GET_STAT_FAILED;
	}

	if(type != 'r' || type != 'c'){
		goto FILE_TYPE_FAILED;
	}

	length = (size + apSize)/FILE_READ_BLOCK*FILE_READ_BLOCK + FILE_READ_BLOCK;
	if((buffer = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) < 0){
		goto BUFFER_MMAP_FAILED;
	}

	size += datafunc(size, buffer, output);

	if(length < size){
		size = length;
		goto FILE_SIZE_FAILED;
	}

	if(ftruncate(fd, size) < 0){
		goto FILE_TRUNC_FAILED;
	}

	ret = 0;
FILE_TRUNC_FAILED:
FILE_SIZE_FAILED:
	munmap(buffer, size);
BUFFER_MMAP_FAILED:
FILE_TYPE_FAILED:
FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return ret;
}

int readDir(char* filename, DataFunc dirfunc, void* output)
{
	int ret = -1;
	char type = 'x';
	off_t size = 0;
	int fd = -1;
	DIR* dfd = NULL;
	struct dirent* entry = NULL;
	
	if((fd = open(filename, O_RDONLY)) < 0){
		goto FILE_OPEN_FAILED;
	}

	if(getFileStats(fd, &type, &size) < 0){
		goto FILE_GET_STAT_FAILED;
	}

	if(type != 'd'){
		goto FILE_TYPE_FAILED;
	}

	if((dfd = fdopendir(fd)) == NULL){
		goto DIR_OPEN_FAILED;
	}

	while(entry = readdir(dfd)){
		if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))continue;

		switch(entry->d_type){
			case DT_REG:type = 'r';break;
			case DT_DIR:type = 'd';break;
			case DT_LNK:type = 'l';break;
			case DT_SOCK:type = 's';break;
			case DT_CHR:type = 'c';break;
			case DT_BLK:type = 'b';break;
			case DT_FIFO:type = 'f';break;
			case DT_UNKNOWN:type = 'x';break;
		}

		if(dirfunc(type, filename, entry->d_name, output))break;
	}
	dirfunc('d', filename, ".", output);

	ret = 0;
	closedir(dfd);
DIR_OPEN_FAILED:
FILE_TYPE_FAILED:
FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return ret;
}

int writeFile(char* filename, char* buffer, off_t size, char append)
{
	int ret = -1;
	int fd = -1;
	char type = 'x';
	off_t orgSize = 0;
	int flags = O_WRONLY|O_CREAT;

	append && (flags |= O_APPEND);
	if((fd = open(filename, flags, 0640)) < 0){
		goto FILE_OPEN_FAILED;
	}

	if(getFileStats(fd, &type, &orgSize) < 0){
		goto FILE_GET_STAT_FAILED;
	}

	if(type != 'r'){
		goto FILE_TYPE_FAILED;
	}

	ret = write(fd, buffer, size);
	fsync(fd);

FILE_TYPE_FAILED:
FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return ret;
}

