#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "dirent.h"
#include "skiEvent.h"
#include "skiFileIO.h"

int getFileStats(int fd, char* type, off_t* size)
{
	struct stat sb;
	if(fstat(fd, &sb) < 0){
		PGOTO(FILE_STAT_FAILED);
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
			*size = FILE_READ_BLOCK * FILE_READ_COUNT;
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

int getFilenameStats(char* filename, char* type, off_t* size)
{
	int fd = 0;
	if((fd = open(filename, O_RDONLY)) < 0){
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, type, size) < 0){
		PGOTO(FILE_STAT_FAILED);
	}

	return fd;

FILE_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return -1;
}

static int dataFuncDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc != 4){
		PGOTO(FUNC_ARGS_FAILED);
	}
	int fd = (int)argv[0];
	off_t size = (off_t)argv[1];
	DataFunc datafunc = argv[2];
	char* output = argv[3];

	char* buffer = NULL;
	off_t readSize = 0;
	off_t rebacks = 0;
	if((buffer = malloc(FILE_READ_BLOCK)) == NULL){
		PGOTO(BUTTER_MOLLOC_FAILED);
	}

	memset(buffer, 0, FILE_READ_BLOCK);
	while((readSize = read(fd, buffer, FILE_READ_BLOCK)) > 0){
		rebacks = datafunc(readSize, buffer, output);
		if(rebacks > 0 && rebacks < readSize){
			lseek(fd, 0 - rebacks, SEEK_CUR);
		}else if(rebacks < 0){
			PGOTO(FUNC_CALL_FAILED);
		}
	}
	datafunc(0, buffer, output);

	ret = 0;

FUNC_CALL_FAILED:
	free(buffer);
	buffer = NULL;
BUTTER_MOLLOC_FAILED:
	close(fd);
FUNC_ARGS_FAILED:
	return ret;
}

int readFile(char* filename, DataFunc datafunc, void* dataout)
{
	char type = 'x';
	off_t size = 0;
	int fd = -1;

	if(datafunc == NULL){
		PGOTO(FUNC_ARGS_FAILED);
	}

	if((fd = open(filename, O_RDONLY)) < 0){
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, &type, &size) < 0){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	if(type != 'r' || type != 'c' || type != 'f'){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	void* argv[4];
	argv[0] = (void*)fd;
	argv[1] = (void*)size;
	argv[2] = datafunc;
	argv[3] = dataout;
	if(isThreadPool()){
		setThreadCallbackRaw(dataFuncDecorator, 4, argv);
		//setThreadCallback(dataFuncDecorator, 4, fd, size, datafunc, dataout);
	}else{
		dataFuncDecorator(4, argv);
	}

	return 0;

FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
FUNC_ARGS_FAILED:
	return -1;
}

static int lineDataFuncDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc != 4){
		PGOTO(FUNC_ARGS_FAILED);
	}
	int fd = (int)argv[0];
	off_t size = (off_t)argv[1];
	DataFunc datafunc = argv[2];
	char* output = argv[3];

	char* buffer = NULL;
	off_t readSize = 0;
	off_t rebacks = 0;
	if((buffer = malloc(FILE_READ_BLOCK)) == NULL){
		PGOTO(BUTTER_MOLLOC_FAILED);
	}

	int idx = 0;
	int i;
	char* cursor = NULL;
	memset(buffer, 0, FILE_READ_BLOCK);
	while((readSize = read(fd, buffer, FILE_READ_BLOCK)) > 0){
		cursor = buffer;
		for(i = 0; i < readSize; i++){
			if(buffer[i] == 10){
				buffer[i] = 0;
				if(datafunc(idx++, cursor, output) < 0){
					break;
				}
				cursor = buffer + i + 1;
			}
		}
		rebacks = buffer + i - cursor;
		readSize > rebacks && lseek(fd, 0 - rebacks, SEEK_CUR);
	}
	datafunc(-1, buffer, output);

	ret = 0;

	free(buffer);
	buffer = NULL;
BUTTER_MOLLOC_FAILED:
	close(fd);
FUNC_ARGS_FAILED:
	return ret;
}

int readFileLine(char* filename, DataFunc datafunc, void* dataout)
{
	char type = 'x';
	off_t size = 0;
	int fd = -1;

	if(datafunc == NULL){
		PGOTO(FUNC_ARGS_FAILED);
	}

	if((fd = open(filename, O_RDONLY)) < 0){
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, &type, &size) < 0){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	if(type != 'r'){
		PGOTO(FILE_GET_STAT_FAILED);
	}


	void* argv[4];
	argv[0] = (void*)fd;
	argv[1] = (void*)size;
	argv[2] = datafunc;
	argv[3] = dataout;
	if(isThreadPool()){
		setThreadCallbackRaw(lineDataFuncDecorator, 4, argv);
		//setThreadCallback(dataFuncDecorator, 4, fd, size, datafunc, dataout);
	}else{
		lineDataFuncDecorator(4, argv);
	}

	return 0;

FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
FUNC_ARGS_FAILED:
	return -1;
}

static int allDataFuncDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc != 4){
		PGOTO(FUNC_ARGS_FAILED);
	}
	int fd = (int)argv[0];
	off_t size = (off_t)argv[1];
	DataFunc datafunc = argv[2];
	char* output = argv[3];

	char* buffer = NULL;
	off_t length = size/FILE_READ_BLOCK + FILE_READ_BLOCK*FILE_READ_COUNT;
	if((buffer = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) < 0){
		PGOTO(BUFFER_MMAP_FAILED);
	}

	size += datafunc(size, buffer, output);

	if(length < size){
		PGOTO(FILE_SIZE_FAILED);
	}

	if(ftruncate(fd, size) < 0){
		PGOTO(FILE_TRUNC_FAILED);
	}

	ret = 0;

FILE_TRUNC_FAILED:
FILE_SIZE_FAILED:
	munmap(buffer, size);
BUFFER_MMAP_FAILED:
	close(fd);
FUNC_ARGS_FAILED:
	return ret;
}

int readFileAll(char* filename, DataFunc datafunc, void* dataout)
{
	char type = 'x';
	off_t size = 0;
	int fd = -1;

	if(datafunc == NULL){
		PGOTO(FUNC_ARGS_FAILED);
	}

	if((fd = open(filename, O_RDWR|O_CREAT, 0640)) < 0){
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, &type, &size) < 0){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	if(type != 'r' || type != 'c'){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	void* argv[4];
	argv[0] = (void*)fd;
	argv[1] = (void*)size;
	argv[2] = datafunc;
	argv[3] = dataout;
	if(isThreadPool()){
		setThreadCallbackRaw(allDataFuncDecorator, 4, argv);
		//setThreadCallback(dataFuncDecorator, 4, fd, size, datafunc, dataout);
	}else{
		allDataFuncDecorator(4, argv);
	}

	return 0;

FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
FUNC_ARGS_FAILED:
	return -1;
}

static int dirFuncDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc != 5){
		PGOTO(FUNC_ARGS_FAILED);
	}
	int fd = (int)argv[0];
	off_t size = (off_t)argv[1];
	DataFunc dirfunc = argv[2];
	char* output = argv[3];
	char* filename = argv[4];

	DIR* dfd = NULL;
	struct dirent* entry = NULL;
	char type = 'x';
	if((dfd = fdopendir(fd)) == NULL){
		PGOTO(DIR_OPEN_FAILED);
	}

	while(entry = readdir(dfd)){
		if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
			continue;
		}
		switch(entry->d_type){
			case DT_REG:
				type = 'r';
				break;
			case DT_DIR:
				type = 'd';
				break;
			case DT_LNK:
				type = 'l';
				break;
			case DT_SOCK:
				type = 's';
				break;
			case DT_CHR:
				type = 'c';
				break;
			case DT_BLK:
				type = 'b';
				break;
			case DT_FIFO:
				type = 'f';
				break;
			case DT_UNKNOWN:
				type = 'x';
				break;
		}
		dirfunc(type, filename, entry->d_name, output);
	}
	dirfunc('d', filename, ".", output);

	ret = 0;

	closedir(dfd);
DIR_OPEN_FAILED:
	free(filename);
	close(fd);
FUNC_ARGS_FAILED:
	return ret;
}

int readDir(char* filename, DataFunc dirfunc, void* dataout)
{
	char type = 'x';
	off_t size = 0;
	int fd = -1;
	char* namebuf = NULL;

	if(dirfunc == NULL){
		PGOTO(FUNC_ARGS_FAILED);
	}

	if((namebuf = malloc(strlen(filename) + 1)) == NULL){
		PGOTO(BUFFER_MALLOC_FAILED);
	}
	strcpy(namebuf, filename);
	
	if((fd = open(filename, O_RDONLY)) < 0){
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, &type, &size) < 0){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	if(type != 'd'){
		PGOTO(FILE_GET_STAT_FAILED);
	}


	void* argv[5];
	argv[0] = (void*)fd;
	argv[1] = (void*)size;
	argv[2] = dirfunc;
	argv[3] = dataout;
	argv[4] = namebuf;
	if(isThreadPool()){
		setThreadCallbackRaw(dirFuncDecorator, 5, argv);
		//setThreadCallback(dataFuncDecorator, 4, fd, size, datafunc, dataout);
	}else{
		dirFuncDecorator(5, argv);
	}

FUNC_PROC_END:
	return 0;

FILE_GET_STAT_FAILED:
FILE_OPEN_FAILED:
	free(namebuf);
BUFFER_MALLOC_FAILED:
FUNC_ARGS_FAILED:
	return -1;
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
		PGOTO(FILE_OPEN_FAILED);
	}

	if(getFileStats(fd, &type, &orgSize) < 0){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	if(type != 'r'){
		PGOTO(FILE_GET_STAT_FAILED);
	}

	ret = write(fd, buffer, size);

FILE_GET_STAT_FAILED:
	close(fd);
FILE_OPEN_FAILED:
	return ret;
}

