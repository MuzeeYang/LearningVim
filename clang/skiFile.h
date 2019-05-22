#ifndef __SKIFILE_H__
#define __SKIFILE_H__

#define FILE_READ_BLOCK (0x1000)
//#define FILE_READ_COUNT (1)

typedef int (*DataFunc)();

//fd: opened file descriptor
//type: out argument, type of file
//size: out argument, size of file
//return: 0 on success, -1 on failed
int getFileStats(int fd, char* type, off_t* size);

//description: read file/charactor/fifo device, every reading by FILE_READ_BLOCK size, c/f device reading endless.
//datafunc: call back function, arguments by (int size, char* buffer, void* output), returnreback bytes or -1 for stopping, in last time size be 0.
//return: 0 on success, -1 on failed
int readFile(char* filename, DataFunc datafunc, void* dataout);

//description: read file by mmap, litte scale change support.
//apSize: accelerating size.
//datafunc: call back function, arguments by (off_t size, char* buffer, void* output), return changed bytes, write back may fail since size out of mmapped
//return: 0 on success, -1 on failed
int readFileAll(char* filename, off_t apSize, DataFunc datafunc, void* output);

//description: list contents of dir. 
//datafunc: call back function, arguments by (char type, char* root, char* name, void* output), return -1 for stopping, in last time name be ".".
//return: 0 on success, -1 on failed
int readDir(char* filename, DataFunc dirfunc, void* dataout);

//description: write buffer to file by synchronizing.
//buffer: buffer for writing
//size: size of buffer for writing
//append: 1 for appending write, 0 for truncating write
//return: writen bytes on success, -1 on failed
int writeFile(char* filename, char* buffer, off_t size, char append);

#endif //skiFile.h
